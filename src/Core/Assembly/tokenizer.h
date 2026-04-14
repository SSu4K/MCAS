#ifndef TOKENIZER_H
#define TOKENIZER_H

namespace Assembly {

enum TokenType{Identifier, Bracket, Register, Hex, Unknown, NotClassified};

struct Token{
    TokenType type;
    QString text;
    qsizetype charIndex;
    qsizetype tokenIndex;
    qsizetype lineNumber;
};

inline Token EMPTY_TOKEN = {TokenType::NotClassified, "", 0, 0, 0};

typedef QList<Token> TokenList;

class Tokenizer
{
private:

    QSet<QChar> separatorChars = {','}; // ignored like spaces
    QSet<QChar> bracketChars = {'(', ')'}; // semantically meaningful
    QSet<QChar> commentChars = {';', '#'};

    Token classifyToken(const Token &token) const;

public:
    TokenList tokenizeLine(const QString &line, const qsizetype lineNumber) const;
    Tokenizer() = default;
};

}

#endif // TOKENIZER_H
