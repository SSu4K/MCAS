#include "assembler.h"

using namespace Assembly;

Assembler::Assembler(const std::shared_ptr<InstructionSet> &instructionSet, const std::shared_ptr<LabelData> &labelData)
    : instructionSet(instructionSet), labelData(labelData) {}

QMap<QString, Token> Assembler::mapTokens(const TokenList &formatTokens, const TokenList &argumentTokens, AssemblyStatus &status) const{
    // this loop assumes the format is correct and validates arguments based on it
    QMap<QString, Token> tokenMappings;
    Token ftok;
    Token atok;
    qsizetype i=0;

    QString msg;
    while(i<formatTokens.size()){
        ftok = formatTokens[i];
        if(i>=argumentTokens.size()){
            msg = "Expected a token for: " + ftok.text + " instead of nothing";
            status = AssemblyStatus::fail(ErrorType::MissingToken, msg);
            return {};
        }
        atok = argumentTokens[i];
        i++;

        // separators like , ( )
        if(ftok.type == TokenType::Bracket){
            if(atok.type != TokenType::Bracket){
                msg = "Expected bracket token instead of: " + atok.text;
                status = AssemblyStatus::fail(ErrorType::WrongToken, msg, atok);
                return {};
            }
            if(ftok.text != atok.text){
                msg = "Expected bracket token: " + ftok.text + " instead of: " + atok.text;
                status = AssemblyStatus::fail(ErrorType::InvalidToken, msg, atok);
                return {};
            }
            continue;
        }
        // registers like R1, R2, R31
        else if((ftok.type == TokenType::Register) && (atok.type != TokenType::Register)){
            msg = "Expected register token for: " + ftok.text + " instead of: " + atok.text;
            status = AssemblyStatus::fail(ErrorType::WrongToken, msg, atok);
            return {};
        }
        // Hexadecimal immediates starting with '0x'
        else if(ftok.text == "i" && atok.type != TokenType::Hex){
            msg = "Expected hex immediate token for: i instead of: " + atok.text;
            status = AssemblyStatus::fail(ErrorType::WrongToken, msg, atok);
            return {};
        }
        // Label type immediate
        else if((ftok.text == "j") && (atok.type != TokenType::Hex) && (atok.type != TokenType::Identifier)){
            msg = "Expected hex or label token for: j instead of: " + atok.text;
            status = AssemblyStatus::fail(ErrorType::WrongToken, msg, atok);
            return {};
        }
        tokenMappings[ftok.text] = atok;
    }

    if(i<argumentTokens.size()){
        status = AssemblyStatus::fail(ErrorType::WrongToken, "Too many tokens for the format", argumentTokens[i]);
        return {};
    }

    status = AssemblyStatus::done("Success");
    return tokenMappings;
}

quint32 Assembler::parseRegisterToken(const Token &token, const qsizetype bitWidth, AssemblyStatus &status) const{
    if(token.type != TokenType::Register){
        status = AssemblyStatus::fail(ErrorType::WrongToken, "Not a register", token);
        return 0;
    }

    bool ok = true;
    QString number = token.text.sliced(1);
    int index = number.toInt(&ok);

    if(!ok){
        QString msg = "Invalid register: " + token.text;
        status = AssemblyStatus::fail(ErrorType::InvalidToken, msg, token);
        return 0;
    }

    const quint32 bitMask = (1u << bitWidth) - 1u;
    if(index < 0 || index > bitMask){
        QString msg = "Register index out of range";
        status = AssemblyStatus::fail(ErrorType::InvalidToken, msg, token);
        return 0;
    }

    status = AssemblyStatus::done("Parsed register token");
    return bitMask & index;
}

quint32 Assembler::parseHexToken(const Token &token, const qsizetype bitWidth, AssemblyStatus &status) const{
    if(token.type != TokenType::Hex){
        status = AssemblyStatus::fail(ErrorType::WrongToken, "Not a hex", token);
        return 0;
    }

    bool ok;
    uint32_t value = token.text.toUInt(&ok, 16);
    if (!ok) {
        QString msg = "Invalid hex immediate: " + token.text;
        status = AssemblyStatus::fail(ErrorType::InvalidToken, msg, token);
        return 0;
    }

    // Validate that it fits into bitWidth
    const quint32 bitMask = (1u << bitWidth) - 1u;

    if (value > bitMask) {
        QString msg = QString("Immediate out of range for %1-bit hex immediate field").arg(bitWidth);
        status = AssemblyStatus::fail(ErrorType::InvalidToken, msg, token);
        return 0;
    }
    status = AssemblyStatus::done("Parsed hex token");
    return bitMask & value;
}

quint32 Assembler::parseLabelToken(const Token &token, const qsizetype bitWidth, AssemblyStatus &status) const{
    if(token.type != TokenType::Identifier){
        status = AssemblyStatus::fail(ErrorType::WrongToken, "Not a label", token);
        return 0;
    }
    bool ok;
    quint32 labelAdress = labelData->getAddress(token.text, &ok);
    if(!ok){
        QString msg = "Unknown label: " + token.text;
        status = AssemblyStatus::fail(ErrorType::InvalidToken, msg, token);
        return 0;
    }
    else {
        const quint32 bitMask = (1u << bitWidth) - 1u;
        const qint32 offset = labelAdress - 4*(token.lineNumber+1); // next line is offset = 0
        const qint32 modulus = 1 << (bitWidth-1); // modulus for 2c
        const qint32 minValue = -modulus;
        const qint32 maxValue = modulus - 1;

        qDebug() << labelAdress << token.lineNumber << offset << modulus;

        // jump out of range
        if (offset < minValue || offset > maxValue) {
            QString msg = "Jump to label: " + token.text + " out of range for I-Type encoding";
            status = AssemblyStatus::fail(ErrorType::InvalidToken, msg, token);
            return 0;
        }

        status = AssemblyStatus::done("Parsed label token");
        // save immediate in 2c
        if(offset >= 0){
            return bitMask & offset;
        }
        else{
            return bitMask & (offset+modulus);
        }
    }
}

quint32 Assembler::parseJumpToken(const Token &token, const qsizetype bitWidth, AssemblyStatus &status) const{
    quint32 result = 0;
    if(token.type == TokenType::Hex){
        result = parseHexToken(token, bitWidth, status);
    }
    else if(token.type == TokenType::Identifier){
        result = parseLabelToken(token, bitWidth, status);
    }
    else{
        status = AssemblyStatus::fail(ErrorType::WrongToken, "Incorrect jump token", token);
    }
    return result;
}

RType Assembler::parseRType(const QMap<QString, Token> &tokenMappings, const InstructionDefinition *definition, AssemblyStatus &status) const{
    QList<quint8> regs = {};
    for(qsizetype i=1; i<=encodingConfig->RFormalCount(); i++){
        QString ftok = QString("r%1").arg(i, 0, 10);
        if(tokenMappings.contains(ftok)){
            Token atok = tokenMappings[ftok];
            quint8 reg = parseRegisterToken(atok, encodingConfig->registerSize(), status);
            if(!status.isOk()){
                return RType();
            }
            regs.append(reg);

        }
        else{
            regs.append(0);
        }
    }

    quint32 opcode = instructionSet->getOpcode(definition->mnemonic, nullptr);
    status = AssemblyStatus::done("Parsed RType instruction");
    return RType(opcode, regs);
}

IType Assembler::parseIType(const QMap<QString, Token> &tokenMappings, const InstructionDefinition *definition, AssemblyStatus &status) const{
    quint8 r1 = 0;
    if(tokenMappings.contains("r1")){
        Token token = tokenMappings["r1"];
        r1 = parseRegisterToken(token, encodingConfig->registerSize(), status);
        if(!status.isOk()){
            return IType();
        }
    }

    quint8 r2 = 0;
    if(tokenMappings.contains("r2")){
        Token token = tokenMappings["r2"];
        r2 = parseRegisterToken(token, encodingConfig->registerSize(), status);
        if(!status.isOk()){
            return IType();
        }
    }

    uint32_t immediate = 0;

    // I-Type with hexadecimal immediate (example: ADDI R2, 0x0022(R1))
    if(tokenMappings.contains("i")){
        Token token = tokenMappings["i"];
        immediate = parseHexToken(token, encodingConfig->IImmediateSize(), status);
        if(!status.isOk()){
            return IType();
        }
    }
    // I-Type with lebel immediate (example: BRZ R1, loop)
    else if(tokenMappings.contains("j")){
        Token token = tokenMappings["j"];
        immediate = parseJumpToken(token, encodingConfig->IImmediateSize(), status);
        if(!status.isOk()){
            return IType();
        }
    }

    quint32 opcode = instructionSet->getOpcode(definition->mnemonic, nullptr);
    status = AssemblyStatus::done("Parsed IType instruction");
    return IType(opcode, r1, r2, immediate);
}

JType Assembler::parseJType(const QMap<QString, Token> &tokenMappings, const InstructionDefinition *definition, AssemblyStatus &status) const{
    uint32_t immediate = 0;
    if(tokenMappings.contains("j")){
        Token token = tokenMappings["j"];
        immediate = parseJumpToken(token, encodingConfig->JImmediateSize(), status);
        if(!status.isOk()){
            return JType();
        }
    }
    quint32 opcode = instructionSet->getOpcode(definition->mnemonic, nullptr);

    status = AssemblyStatus::done("Parsed JType instruction");
    return JType(opcode, immediate);
}

std::shared_ptr<Instruction> Assembler::assembleLine(const QString &instruction, const qsizetype lineNumber, AssemblyStatus &status) const{
    QString trimmed = instruction.trimmed();
    if(trimmed.isEmpty()){
        status = AssemblyStatus::fail(ErrorType::MissingToken, "Empty line");
        return nullptr;
    }

    TokenList argumentTokens = tokenizer.tokenizeLine(trimmed, lineNumber);

    // Check if first token is an identifier (label)
    if(argumentTokens[0].type != TokenType::Identifier){
        status = AssemblyStatus::fail(ErrorType::WrongToken, "No mnemonic");
        return nullptr;
    }

    // Recognize the instrucion
    QString mnemonic = argumentTokens[0].text;
    const InstructionDefinition *definition = instructionSet->getDefinition(mnemonic);
    if(definition == nullptr){
        status = AssemblyStatus::fail(ErrorType::InvalidToken, "Unknown instruction");
        return nullptr;
    }

    // remove mnemonic from arguments for parsing
    argumentTokens.removeFirst();
    // tokenize format string
    TokenList formatTokens = definition->formatTokens;

    // map format token strings to argument tokens
    QMap<QString, Token> tokenMappings = mapTokens(formatTokens, argumentTokens, status);

    if(status.severity == ErrorSeverity::Error){
        return nullptr;
    }

    // qDebug() << "Parsed:" << instruction;
    // qDebug() << "Mnemonic:" << mnemonic << "Type:" << (int)definition.type << "Opcode:" << definition.opcode << "Format:" << definition.format;
    // for(auto [key, value]: tokenMappings.asKeyValueRange()){
    //     qDebug() << key << "->" << value.str;
    // }

    if(definition->type == InstructionType::R){
        RType r = parseRType(tokenMappings, definition, status);
        return std::make_shared<RType>(r);
    }
    else if(definition->type == InstructionType::I){
        IType i = parseIType(tokenMappings, definition, status);
        return std::make_shared<IType>(i);
    }
    else if(definition->type == InstructionType::J){
        JType j = parseJType(tokenMappings, definition, status);
        return std::make_shared<JType>(j);
    }

    status = AssemblyStatus::fail(ErrorType::Unknown, "Unknown parse error occured");
    return nullptr;
}
