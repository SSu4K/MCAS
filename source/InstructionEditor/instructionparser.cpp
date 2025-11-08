#include "instructionparser.h"
#include <qvariant.h>
#include <QRegularExpression>

using namespace InstructionEditor;

bool isLabel(QString str){
    return QRegularExpression("^[A-Za-z_][A-Za-z0-9_]*$").match(str).hasMatch();
}

ParseStatus::ParseStatus(ErrorSeverity severity, const QString &msg, const Token &token):
    severity(severity), msg(msg), token(token) {}

ParseStatus ParseStatus::done(const QString &msg, const Token &token){
    return ParseStatus(ErrorSeverity::Correct, msg, token);
}

ParseStatus ParseStatus::warning(const QString &msg, const Token &token){
    return ParseStatus(ErrorSeverity::Warning, msg, token);
}

ParseStatus ParseStatus::fail(const QString &msg, const Token &token){
    return ParseStatus(ErrorSeverity::Error, msg, token);
}

QString ParseStatus::toString() const {
    QString rankStr;
    switch (severity) {
    case ErrorSeverity::Correct:    rankStr = "Correct"; break;
    case ErrorSeverity::Warning:    rankStr = "Warning"; break;
    case ErrorSeverity::Error:      rankStr = "Error"; break;
    }

    QString loc;
    if (token.tokenIndex >= 0 || token.charIndex >= 0)
        loc = QString(" [token=%1 line=%2 index=%3 char=%4]").arg(token.str).arg(token.lineNumber).arg(token.tokenIndex).arg(token.charIndex);

    if (!msg.isEmpty())
        return QString("%1: %2%3").arg(rankStr, msg, loc);
    return QString("%1%2").arg(rankStr, loc);
}

bool ParseStatus::isOk() const{
    return severity == ErrorSeverity::Correct;
}

bool ParseStatus::isWarning() const{
    return severity == ErrorSeverity::Warning;
}

bool ParseStatus::isError() const{
    return severity == ErrorSeverity::Error;
}

QDebug InstructionEditor::operator<<(QDebug dbg, const ParseStatus &status)
{
    QDebugStateSaver saver(dbg);
    dbg.nospace() << status.toString();
    return dbg;
}

bool InstructionParser::addInstruction(const QString &mnemonic, const InstructionType type, const QString &format){
    if(instructionSet.contains(mnemonic.toUpper())){
        return false;
    }
    quint8 opcode = instructionSet.size();
    InstructionDefinition definition = {opcode, type, format};
    instructionSet[mnemonic.toUpper()] = definition;
    return true;
}

bool InstructionParser::removeInstruction(const QString &mnemonic){
    return instructionSet.remove(mnemonic);
}

bool InstructionParser::addLabel(const QString &label, const qsizetype lineNumber){
    if(labelMap.contains(label)){
        return false;
    }
    labelMap[label] = lineNumber;
    return true;
}

bool InstructionParser::removeLabel(const QString &label){
    return labelMap.remove(label);
}

InstructionParser::InstructionParser(): instructionSet(DEFAULT_INSTRUCTION_SET), separatorTokens(DEFAULT_SEPARATOR_TOKENS) {}

void InstructionParser::flushCharBuffer(){
    if(!charBuffer.isEmpty()){
        tokenBuffer.append({charBuffer, currentCharIndex, tokenBuffer.size()+1, currentLineNumber});
        charBuffer = "";
    }
}

TokenList InstructionParser::tokenize(const qsizetype lineNumber, const QString &line){
    currentCharIndex = 0;
    charBuffer = 0;
    tokenBuffer = {};
    for(qsizetype i=0; i<line.size(); i++){
        QChar c = line[i];
        if(c.isSpace()){
            flushCharBuffer();
            currentCharIndex = i+1;
        }
        else if(separatorTokens.contains(c)){
            flushCharBuffer();
            charBuffer = c;
            currentCharIndex = i;
            flushCharBuffer();
            currentCharIndex = i+1;
        }
        else{
            charBuffer += c;
        }
    }
    flushCharBuffer();
    return tokenBuffer;
}

ParseStatus InstructionParser::mapTokens(const TokenList &formatTokens, const TokenList &argumentTokens, QMap<QString, Token> &tokenMappings) const{
    // this loop assumes the format is correct and validates arguments based on it
    Token ftok;
    Token atok;
    qsizetype i=0;

    QString msg;
    while(i<formatTokens.size()){
        ftok = formatTokens[i];
        if(i>=argumentTokens.size()){
            msg = "Expected a token for: " + ftok.str + " instead of nothing";
            return ParseStatus::fail(msg);
        }
        atok = argumentTokens[i];
        i++;
        //charIndex += atok.

        // separators like , ( )
        if(separatorTokens.contains(ftok.str)){
            if(ftok.str != atok.str){
                msg = "Expected separator token: " + ftok.str + " instead of: " + atok.str;
                return ParseStatus::fail(msg, atok);
            }
            continue;
        }
        // registers like R1, R2, R31
        else if(ftok.str.startsWith("r")){
            if (!atok.str.toUpper().startsWith("R")){
                msg = "Expected register token for: " + ftok.str + " instead of: " + atok.str;
                return ParseStatus::fail(msg, atok);
            }
        }
        // Hexadecimal immediates starting with '0x'
        else if(ftok.str == 'i'){
            if(!atok.str.startsWith("0x")){
                msg = "Expected hex immediate token for: i instead of: " + atok.str;
                return ParseStatus::fail(msg, atok);
            }
        }
        // Label type immediate
        else if(ftok.str == "j"){
            if(!isLabel(atok.str)){
                msg = "Invalid label: " + atok.str;
                return ParseStatus::fail(msg, atok);
            }
        }
        tokenMappings[ftok.str] = atok;
    }

    if(i<argumentTokens.size()){
        msg = "Too many tokens for the format";
        return ParseStatus::fail(msg, argumentTokens[i]);
    }

    msg = "Success!";
    return ParseStatus::done(msg);
}

quint32 InstructionParser::parseRegisterToken(const Token &token, const qsizetype bitWidth, ParseStatus &status) const{
    if(!token.str.startsWith('R')){
        QString msg = "Invalid register: " + token.str;
        status = ParseStatus::fail(msg, token);
        return 0;
    }
    bool ok = true;
    QString number = token.str.sliced(1);
    int index = number.toInt(&ok);

    if(!ok){
        QString msg = "Invalid register: " + token.str;
        status = ParseStatus::fail(msg, token);
        return 0;
    }

    const quint32 bitMask = (1u << bitWidth) - 1u;
    if(index < 0 || index > bitMask){
        QString msg = "Register index out of range";
        status = ParseStatus::fail(msg, token);
        return 0;
    }

    status = ParseStatus::done("Parsed register token");
    return bitMask & index;
}

quint32 InstructionParser::parseHexToken(const Token &token, const qsizetype bitWidth, ParseStatus &status) const{
    if(!token.str.startsWith("0x", Qt::CaseInsensitive)){
        QString msg = "Invalid hex immediate: " + token.str;
        status = ParseStatus::fail(msg, token);
        return 0;
    }
    bool ok;

    uint32_t value = token.str.toUInt(&ok, 16);
    if (!ok) {
        QString msg = "Invalid hex immediate: " + token.str;
        status = ParseStatus::fail(msg, token);
        return 0;
    }

    // Validate that it fits into bitWidth
    const quint32 bitMask = (1u << bitWidth) - 1u;

    if (value > bitMask) {
        QString msg = QString("Immediate out of range for %1-bit hex immediate field").arg(bitWidth);
        status = ParseStatus::fail(msg, token);
        return 0;
    }
    status = ParseStatus::done("Parsed hex token");
    return bitMask & value;
}

quint32 InstructionParser::parseLabelToken(const Token &token, const qsizetype bitWidth, ParseStatus &status) const{
    if(!labelMap.contains(token.str)){
        QString msg = "Unknown label: " + token.str;
        status = ParseStatus::fail(msg, token);
        return 0;
    }
    else {
        const quint32 bitMask = (1u << bitWidth) - 1u;
        const qint32 offset = 4*(labelMap[token.str] - (token.lineNumber+1)); // next line is offset = 0
        const quint32 modulus = 1 << (bitWidth-1); // modulus for 2c
        const qint32 minValue = -modulus;
        const quint32 maxValue = modulus - 1;

        // jump out of range
        if (offset < minValue || offset > maxValue) {
            QString msg = "Jump to label: " + token.str + " out of range for I-Type encoding";
            status = ParseStatus::fail(msg, token);
            return 0;
        }

        status = ParseStatus::done("Parsed label token");
        // save immediate in 2c
        if(offset >= 0){
            return bitMask & offset;
        }
        else{
            return bitMask & (offset+modulus);
        }
    }
}

quint32 InstructionParser::parseJumpToken(const Token &token, const qsizetype bitWidth, ParseStatus &status) const{
    quint32 result = 0;
    result = parseHexToken(token, bitWidth, status);
    if(status.isOk()){
        return result;
    }
    result = parseLabelToken(token, bitWidth, status);
    if(status.isOk()){
        return result;
    }
    else{
        return 0;
    }
}

ParseResult InstructionParser::parseRType(const QMap<QString, Token> &tokenMappings, const InstructionDefinition &definition) const{
    QList<quint8> regs = {};
    ParseStatus status;
    for(qsizetype i=1; i<=R_FORMAL_COUNT; i++){
        QString ftok = QString("r%1").arg(i, 0, 10);
        if(tokenMappings.contains(ftok)){
            Token atok = tokenMappings[ftok];
            quint8 reg = parseRegisterToken(atok, REGISTER_SIZE, status);
            if(!status.isOk()){
                return {nullptr, status};
            }
            regs.append(reg);

        }
        else{
            regs.append(0);
        }
    }

    auto instr_ptr = QSharedPointer<Instruction>(new RType(definition.opcode, regs));
    return {instr_ptr, ParseStatus::done("Parsed RType instruction")};
}

ParseResult InstructionParser::parseIType(const QMap<QString, Token> &tokenMappings, const InstructionDefinition &definition) const{
    ParseStatus status;
    quint8 r1 = 0;
    if(tokenMappings.contains("r1")){
        Token token = tokenMappings["r1"];
        r1 = parseRegisterToken(token, REGISTER_SIZE, status);
        if(!status.isOk()){
            return {nullptr, status};
        }
    }

    quint8 r2 = 0;
    if(tokenMappings.contains("r2")){
        Token token = tokenMappings["r2"];
        r2 = parseRegisterToken(token, REGISTER_SIZE, status);
        if(!status.isOk()){
            return {nullptr, status};
        }
    }

    uint32_t immediate = 0;

    // I-Type with hexadecimal immediate (example: ADDI R2, 0x0022(R1))
    if(definition.format.contains('i')){
        Token token = tokenMappings["i"];
        immediate = parseHexToken(token, I_IMMEDIATE_SIZE, status);
        if(!status.isOk()){
            return {nullptr, status};
        }
    }
    // I-Type with lebel immediate (example: BRZ R1, loop)
    else if(definition.format.contains('j')){
        Token token = tokenMappings["j"];
        immediate = parseLabelToken(token, I_IMMEDIATE_SIZE, status);
        if(!status.isOk()){
            return {nullptr, status};
        }
    }

    auto instr_ptr = QSharedPointer<Instruction>(new IType(definition.opcode, r1, r2, immediate));
    return {instr_ptr, ParseStatus::done("Parsed IType instruction")};
}

ParseResult InstructionParser::parseJType(const QMap<QString, Token> &tokenMappings, const InstructionDefinition &definition) const{
    bool ok = true;
    uint32_t immediate = 0;
    if(tokenMappings.contains("j")){
        Token token = tokenMappings["j"];



    }

    auto instr_ptr = QSharedPointer<Instruction>(new JType(definition.opcode, immediate));
    return {instr_ptr, ParseStatus::done("Parsed JType instruction")};
}

ParseResult InstructionParser::parseLine(const qsizetype lineNumber, const QString &instruction){
    currentLineNumber = lineNumber;

    QString trimmed = instruction.trimmed();
    if(trimmed.isEmpty()){
        return {nullptr, ParseStatus::fail("Empty line")};
    }

    TokenList argumentTokens = tokenize(lineNumber, trimmed);

    // Recognize the instrucion
    QString mnemonic = argumentTokens[0].str;
    if(!instructionSet.contains(mnemonic)){
        return {nullptr, ParseStatus::fail("Unknown instruction")};
    }
    const auto &definition = instructionSet[mnemonic];

    // remove mnemonic from arguments for parsing
    argumentTokens.removeFirst();
    // tokenize format string
    TokenList formatTokens = tokenize(0, definition.format); // lineNumber irrelevant for format tokens

    // map format token strings to argument tokens
    QMap<QString, Token> tokenMappings;
    auto status = mapTokens(formatTokens, argumentTokens, tokenMappings);

    if(status.severity == ErrorSeverity::Error){
        return {nullptr, status};
    }

    qDebug() << "Parsed:" << instruction;
    qDebug() << "Mnemonic:" << mnemonic << "Type:" << (int)definition.type << "Opcode:" << definition.opcode << "Format:" << definition.format;
    for(auto [key, value]: tokenMappings.asKeyValueRange()){
        qDebug() << key << "->" << value.str;
    }

    if(definition.type == InstructionType::R){
        return parseRType(tokenMappings, definition);
    }
    else if(definition.type == InstructionType::I){
        return parseIType(tokenMappings, definition);
    }


    return {nullptr, ParseStatus::done("Parsed something")};
}
