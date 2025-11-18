#include "assemblystatus.h"

AssemblyStatus::AssemblyStatus(const ErrorSeverity severity, const ErrorType type, const QString &msg, const Token &token):
    severity(severity), type(type), msg(msg), token(token) {}

AssemblyStatus AssemblyStatus::done(const QString &msg, const Token &token){
    return AssemblyStatus(ErrorSeverity::Correct, ErrorType::None, msg, token);
}

AssemblyStatus AssemblyStatus::warning(const QString &msg, const Token &token){
    return AssemblyStatus(ErrorSeverity::Warning, ErrorType::None, msg, token);
}

AssemblyStatus AssemblyStatus::fail(const ErrorType type, const QString &msg, const Token &token){
    return AssemblyStatus(ErrorSeverity::Error, type, msg, token);
}

QString AssemblyStatus::toString() const {
    QString rankStr;
    switch (severity) {
    case ErrorSeverity::Correct:    rankStr = "Correct"; break;
    case ErrorSeverity::Warning:    rankStr = "Warning"; break;
    case ErrorSeverity::Error:      rankStr = "Error"; break;
    }

    QString errStr;
    if(severity == ErrorSeverity::Error){
        switch (type) {
        case ErrorType::InvalidToken:   errStr = "Invalid"; break;
        case ErrorType::WrongToken:     errStr = "Wrong"; break;
        case ErrorType::MissingToken:   errStr = "Missing"; break;
        case ErrorType::None:           errStr = "None"; break;
        case ErrorType::Unknown:        errStr = "Unknown"; break;
        }
    }

    QString typeStr;
    switch (token.type) {
    case TokenType::Identifier:     typeStr = "Identifier"; break;
    case TokenType::Hex:            typeStr = "Hex"; break;
    case TokenType::Bracket:        typeStr = "Bracket"; break;
    case TokenType::Register:       typeStr = "Register"; break;
    case TokenType::Unknown:        typeStr = "Unknown"; break;
    case TokenType::NotClassified:  typeStr = "NotClassified"; break;
    }

    QString loc;
    if (token.tokenIndex >= 0 || token.charIndex >= 0)
        loc = QString(" [token=%1 type=%2 line=%3 index=%4 char=%5]").arg(token.text).arg(typeStr).arg(token.lineNumber).arg(token.tokenIndex).arg(token.charIndex);

    if (!msg.isEmpty())
        return QString("%1 %2: %3%4").arg(rankStr, errStr, msg, loc);
    return QString("%1%2").arg(rankStr, loc);
}

bool AssemblyStatus::isOk() const{
    return severity == ErrorSeverity::Correct;
}

bool AssemblyStatus::isWarning() const{
    return severity == ErrorSeverity::Warning;
}

bool AssemblyStatus::isError() const{
    return severity == ErrorSeverity::Error;
}

QDebug operator<<(QDebug dbg, const AssemblyStatus &status)
{
    QDebugStateSaver saver(dbg);
    dbg.nospace() << status.toString();
    return dbg;
}
