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
    if (token.index >= 0 || token.address >= 0)
        loc = QString(" [token=%1 index=%2 char=%3]").arg(token.str).arg(token.index).arg(token.address);

    if (!msg.isEmpty())
        return QString("%1: %2%3").arg(rankStr, msg, loc);
    return QString("%1%2").arg(rankStr, loc);
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

InstructionParser::InstructionParser(): instructionSet(DEFAULT_INSTRUCTION_SET), separatorTokens(DEFAULT_SEPARATOR_TOKENS) {}

void InstructionParser::flushCharBuffer(){
    if(!charBuffer.isEmpty()){
        tokenBuffer.append({charBuffer, parseAddress, tokenBuffer.size()+1});
        charBuffer = "";
    }
}

TokenList InstructionParser::tokenize(const QString &line){
    parseAddress = 0;
    charBuffer = 0;
    tokenBuffer = {};
    for(qsizetype i=0; i<line.size(); i++){
        QChar c = line[i];
        if(c.isSpace()){
            flushCharBuffer();
            parseAddress = i+1;
        }
        else if(separatorTokens.contains(c)){
            flushCharBuffer();
            charBuffer = c;
            parseAddress = i;
            flushCharBuffer();
            parseAddress = i+1;
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

quint8 parseRegisterToken(QString token, bool* okptr = nullptr){
    bool ok = true;
    QString number;
    int index;

    if(!token.startsWith('R'))
        goto err;

    number = token.slice(1);
    index = number.toInt(&ok);
    if(!ok || index<0 || index>=REGISTER_COUNT)
        goto err;

    return index;

    err:
    if(okptr){
        *okptr = false;
    }
    return 0;
}

ParseResult InstructionParser::parse(const QString &instruction){
    QString trimmed = instruction.trimmed();
    if(trimmed.isEmpty()){
        return {nullptr, ParseStatus::fail("Empty line")};
    }

    TokenList argumentTokens = tokenize(trimmed);

    // Recognize the instrucion
    QString mnemonic = argumentTokens[0].str;
    if(!instructionSet.contains(mnemonic)){
        return {nullptr, ParseStatus::fail("Unknown instruction")};
    }
    const auto &definition = instructionSet[mnemonic];

    // remove mnemonic from arguments for parsing
    argumentTokens.removeFirst();
    // tokenize format string
    TokenList formatTokens = tokenize(definition.format);

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
        QList<quint8> regs = {};
        bool ok = true;
        for(qsizetype i=1; i<=R_FORMAL_COUNT; i++){
            QString ftok = QString("r%1").arg(i, 0, 10);
            if(tokenMappings.contains(ftok)){
                Token atok = tokenMappings[ftok];
                regs.append(parseRegisterToken(atok.str, &ok));
                if(!ok){
                    QString msg = "Failed to parse register token: " + atok.str;
                    return {nullptr, ParseStatus::done(msg, atok)};
                }
            }
            else{
                regs.append(0);
            }
        }
        auto instr_ptr = QSharedPointer<Instruction>(new RType(definition.opcode, regs));

        return {instr_ptr, ParseStatus::done("Parsed RType instruction")};
    }


    return {nullptr, ParseStatus::done("Parsed something")};
}
