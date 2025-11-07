#include "instructionparser.h"
#include <qvariant.h>
#include <QRegularExpression>

using namespace InstructionEditor;

bool isLabel(QString str){
    return QRegularExpression("^[A-Za-z_][A-Za-z0-9_]*$").match(str).hasMatch();
}

ParseStatus::ParseStatus(ErrorSeverity severity, QString msg, qsizetype tokenIndex, qsizetype charIndex):
    severity(severity), msg(msg), tokenIndex(tokenIndex), charIndex(charIndex) {}

ParseStatus ParseStatus::done(QString msg, qsizetype tokenIndex, qsizetype charIndex){
    return ParseStatus(ErrorSeverity::Correct, msg, tokenIndex, charIndex);
}

ParseStatus ParseStatus::warning(QString msg, qsizetype tokenIndex, qsizetype charIndex){
    return ParseStatus(ErrorSeverity::Warning, msg, tokenIndex, charIndex);
}

ParseStatus ParseStatus::fail(QString msg, qsizetype tokenIndex, qsizetype charIndex){
    return ParseStatus(ErrorSeverity::Error, msg, tokenIndex, charIndex);
}

InstructionParser::InstructionParser(): instructionSet(DEFAULT_INSTRUCTION_SET), separatorTokens(DEFAULT_SEPARATOR_TOKENS) {}

void InstructionParser::flushCharBuffer(){
    if(!charBuffer.isEmpty()){
        tokenBuffer.append({charBuffer, parseAddress, tokenBuffer.size()});
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
            return ParseStatus::fail(msg, atok.index, atok.address);
        }
        atok = argumentTokens[i];
        i++;
        //charIndex += atok.

        // separators like , ( )
        if(separatorTokens.contains(ftok.str)){
            if(ftok.str != atok.str){
                msg = "Expected separator token: " + ftok.str + " instead of: " + atok.str;
                return ParseStatus::fail(msg, atok.index, atok.address);
            }
            continue;
        }
        // registers like R1, R2, R31
        else if(ftok.str.startsWith("r")){
            if (!atok.str.toUpper().startsWith("R")){
                msg = "Expected register token for: " + ftok.str + " instead of: " + atok.str;
                return ParseStatus::fail(msg, atok.index, atok.address);
            }
        }
        // Hexadecimal immediates starting with '0x'
        else if(ftok.str == 'i'){
            if(!atok.str.startsWith("0x")){
                msg = "Expected hex immediate token for: i instead of: " + atok.str;
                return ParseStatus::fail(msg, atok.index, atok.address);
            }
        }
        // Label type immediate
        else if(ftok.str == "j"){
            if(!isLabel(atok.str)){
                msg = "Invalid label: " + atok.str;
                return ParseStatus::fail(msg, atok.index, atok.address);
            }
        }
        tokenMappings[ftok.str] = atok;
    }

    if(i<argumentTokens.size()){
        msg = "Too many tokens for the format";
        return ParseStatus::fail(msg, atok.index, atok.address);
    }

    msg = "Success!";
    return ParseStatus::done(msg, atok.index, atok.address);
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

    // Recognize the instrucion
    QString mnemonic = trimmed.section(' ', 0, 0).toUpper();
    if(!instructionSet.contains(mnemonic)){
        return {nullptr, ParseStatus::fail("Unknown instruction")};
    }
    const auto &definition = instructionSet[mnemonic];
    QString arguments = trimmed.section(' ', 1);

    // tokenize arguments and format
    TokenList formatTokens = tokenize(definition.format);
    TokenList argumentTokens = tokenize(arguments);

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
                    return {nullptr, ParseStatus::done(msg, atok.index, atok.address)};
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
