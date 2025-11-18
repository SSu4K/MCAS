#include "tokenizer.h"
#include <qregularexpression.h>

// labels or mnemonics like: loop, _private_loop, ADD, LOAD_WORD
static QRegularExpression reLabel("^[A-Za-z_][A-Za-z0-9_]*$");

// Register: R0, r31, R15, etc.
static QRegularExpression reRegister("^R[0-9]+$", QRegularExpression::CaseInsensitiveOption);

// Hex: 0x..., case insensitive
static QRegularExpression reHex("^0x[0-9A-Fa-f]+$");

Tokenizer::Tokenizer() {}

Token Tokenizer::classifyToken(const Token &token){
    if(token.type != TokenType::NotClassified){
        return token;   // pass through if already classifed
    }
    Token result = token;

    if(token.text.size() == 1 && bracketChars.contains(token.text[0])){
        result.type = TokenType::Bracket;
    }
    else if(reLabel.match(token.text).hasMatch()){
        result.type = TokenType::Identifier;
    }
    else if(reRegister.match(token.text).hasMatch()){
        result.type = TokenType::Register;
    }
    else if(reHex.match(token.text).hasMatch()){
        result.type = TokenType::Hex;
    }
    else{
        result.type = TokenType::Unknown;
    }

    return result;
}

void Tokenizer::flushCharBuffer(){
    if(!currentToken.text.isEmpty()){
        tokenBuffer.append(classifyToken(currentToken));
        currentToken.text.clear();
        currentToken.tokenIndex += 1;
    }
}

TokenList Tokenizer::tokenizeLine(const QString &line, const qsizetype lineNumber){
    currentToken = {TokenType::NotClassified, "", 0, 0, lineNumber};
    tokenBuffer = {};
    for(qsizetype i=0; i<line.size(); i++){
        QChar c = line[i];
        if(commentChars.contains(c)){
            break;
        }
        else if(c.isSpace() || separatorChars.contains(c)){
            flushCharBuffer();
            currentToken.charIndex = i+1;
        }
        else if(bracketChars.contains(c)){
            // flush previous token
            flushCharBuffer();

            // create and flush separator
            currentToken.type = TokenType::Bracket; // skip classification
            currentToken.text = c;
            currentToken.charIndex = i;
            flushCharBuffer();

            // update charIndex for next token, reset type
            currentToken.type = TokenType::NotClassified;
            currentToken.charIndex = i+1;
        }
        else{
            currentToken.text += c;
        }
    }

    flushCharBuffer();
    return tokenBuffer;
}
