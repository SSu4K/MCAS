#include "instructionparser.h"
#include <qvariant.h>
#include <QRegularExpression>

using namespace InstructionEditor;

bool isLabel(QString str){
    return QRegularExpression("^[A-Za-z_][A-Za-z0-9_]*$").match(str).hasMatch();
}

InstructionParser::InstructionParser(): instructionSet(DEFAULT_INSTRUCTION_SET), separatorTokens(DEFAULT_SEPARATOR_TOKENS) {}

void InstructionParser::flushCharBuffer(){
    if(!charBuffer.isEmpty()){
        tokenBuffer.append(charBuffer);
        charBuffer = "";
    }
}

QStringList InstructionParser::tokenize(const QString &line){
    charBuffer = 0;
    tokenBuffer = {};
    for(QChar c: line){
        if(c.isSpace()){
            flushCharBuffer();
        }
        else if(separatorTokens.contains(c)){
            flushCharBuffer();
            tokenBuffer.append(c);
        }
        else{
            charBuffer += c;
        }
    }
    flushCharBuffer();
    return tokenBuffer;
}

// QMap<QString, QString> InstructionParser::mapTokens(const QStringList &formatTokens, const QStringList &argumentTokens) const{

// }

QVariant InstructionParser::parse(const QString &instruction){
    QString trimmed = instruction.trimmed();
    if(trimmed.isEmpty()){
        return {"Empty line"};
    }

    // Recognize the instrucion
    QString mnemonic = trimmed.section(' ', 0, 0).toUpper();
    if(!instructionSet.contains(mnemonic)){
        return {"Unknown instruction"};
    }
    const auto &definition = instructionSet[mnemonic];
    QString arguments = trimmed.section(' ', 1);

    // tokenize arguments and format
    QStringList argumentTokens = tokenize(arguments);
    QStringList formatTokens = tokenize(definition.format);

    // if(argumentTokens.size() != formatTokens.size()){
    //     return {"Token count mismatch"};
    // }

    QMap<QString, QString> tokenMappings;
    // this loop assumes the format is correct and validates arguments based on it
    QString ftok;
    QString atok;
    qsizetype i=0;

    while(i<formatTokens.size()){
        ftok = formatTokens[i];
        if(i>=argumentTokens.size()){
            return {"Expected a token for: " + ftok + " instead of nothing"};
        }
        atok = argumentTokens[i];
        i++;

        // separators like , ( )
        if(separatorTokens.contains(ftok)){
            if(ftok != atok){
                return {"Expected separator token: " + ftok + " instead of: " + atok};
            }
            continue;
        }
        // registers like R1, R2, R31
        else if(ftok.startsWith("r")){
            if (!atok.toUpper().startsWith("R")){
                return {"Expected register token for: " + ftok + " instead of: " + atok};
            }
        }
        // Hexadecimal immediates starting with '0x'
        else if(ftok == 'i'){
            if(!atok.startsWith("0x")){
                return {"Expected hex immediate token for: i instead of: " + atok};
            }
        }
        // Label type immediate
        else if(ftok == "j"){
            if(!isLabel(atok)){
                return {"Invalid label: " + atok};
            }
        }
        tokenMappings[ftok] = atok;
    }

    if(i<argumentTokens.size()){
        return {"Too many tokens for the format"};
    }

    qDebug() << "Parsed:" << instruction;
    qDebug() << "Mnemonic:" << mnemonic;
    for(auto [key, value]: tokenMappings.asKeyValueRange()){
        qDebug() << key << "->" << value;
    }

    return {"Done"};
}
