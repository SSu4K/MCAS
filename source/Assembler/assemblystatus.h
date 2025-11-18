#ifndef ASSEMBLYSTATUS_H
#define ASSEMBLYSTATUS_H

#include <QDebug>
#include "tokenizer.h"

enum class ErrorSeverity{
    Correct, Warning, Error
};

enum class ErrorType{
    MissingToken, WrongToken, InvalidToken, Unknown, None
};

class AssemblyStatus{
public:
    ErrorSeverity severity;
    ErrorType type;
    QString msg;
    Token token;

    AssemblyStatus(const ErrorSeverity severity=ErrorSeverity::Correct, const ErrorType type=ErrorType::None, const QString &msg="", const Token &token = EMPTY_TOKEN);

    static AssemblyStatus done(const QString &msg="", const Token &token = EMPTY_TOKEN);
    static AssemblyStatus warning(const QString &msg="", Token const &token = EMPTY_TOKEN);
    static AssemblyStatus fail(const ErrorType type=ErrorType::Unknown, const QString &msg="", Token const &token = EMPTY_TOKEN);

    QString toString() const;
    bool isOk() const;
    bool isWarning() const;
    bool isError() const;
};

QDebug operator<<(QDebug dbg, const AssemblyStatus &status);

#endif // ASSEMBLYSTATUS_H
