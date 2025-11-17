#ifndef INSTRUCTIONPARSER_H
#define INSTRUCTIONPARSER_H

#include <QList>
#include <QHash>
#include <QSharedPointer>
#include <qtypes.h>
#include "InstructionEditor/instruction.h"

namespace InstructionEditor {

    inline QList<char> DEFAULT_SEPARATOR_TOKENS = {',', '(', ')'};

    struct Token{
        QString str;
        qsizetype charIndex;
        qsizetype tokenIndex;
        qsizetype lineNumber;
    };

    typedef QList<Token> TokenList;

    enum class ErrorSeverity{
        Correct, Warning, Error
    };

    class ParseStatus{
    public:
        ErrorSeverity severity;
        QString msg;
        Token token;

        ParseStatus(ErrorSeverity severity=ErrorSeverity::Correct, const QString &msg="", const Token &token = {"", -1, -1});

        static ParseStatus done(const QString &msg="", const Token &token = {"", -1, -1});
        static ParseStatus warning(const QString &msg="", Token const &token = {"", -1, -1});
        static ParseStatus fail(const QString &msg="", Token const &token = {"", -1, -1});

        QString toString() const;
        bool isOk() const;
        bool isWarning() const;
        bool isError() const;
    };

    QDebug operator<<(QDebug dbg, const ParseStatus &status);

    struct ParseResult{
        std::shared_ptr<Instruction> instruction;
        ParseStatus status;
    };

    class InstructionParser
    {
    private:
        std::shared_ptr<const InstructionSet> instructionSet;
        QList<char> separatorTokens;

        QMap<QString, qsizetype> labelMap;

        qsizetype currentLineNumber;
        qsizetype currentCharIndex;
        QString charBuffer;
        TokenList tokenBuffer;

        void flushCharBuffer();
        TokenList tokenize(const qsizetype lineNumber, const QString &line);
        ParseStatus mapTokens(const TokenList &formatTokens, const TokenList &argumentTokens, QMap<QString, Token> &tokenMappings) const;

        quint32 parseRegisterToken(const Token &token, const qsizetype bitWidth, ParseStatus &status) const;
        quint32 parseHexToken(const Token &token, const qsizetype bitWidth, ParseStatus &status) const;
        quint32 parseLabelToken(const Token &token, const qsizetype bitWidth, ParseStatus &status) const;
        quint32 parseJumpToken(const Token &token, const qsizetype bitWidth, ParseStatus &status) const;

        ParseResult parseRType(const QMap<QString, Token> &tokenMappings, const InstructionDefinition &definition) const;
        ParseResult parseIType(const QMap<QString, Token> &tokenMappings, const InstructionDefinition &definition) const;
        ParseResult parseJType(const QMap<QString, Token> &tokenMappings, const InstructionDefinition &definition) const;
    public:

        InstructionParser(const InstructionSet &instructionSet);
        // bool addInstruction(const QString &mnemonic, const InstructionType type, const QString &format);
        // bool removeInstruction(const QString &mnemonic);
        bool addLabel(const QString &label, const qsizetype lineNumber);
        bool removeLabel(const QString &label);
        // bool isValidLabel(const QString &label);
        // quint32 getLabelAddress(const QString &label, bool *okptr);
        ParseResult parseLine(const qsizetype lineNumber, const QString &instruction);
    };

} // namespace InstructionEditor

#endif // INSTRUCTIONPARSER_H
