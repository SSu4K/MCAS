#ifndef INSTRUCTIONPARSER_H
#define INSTRUCTIONPARSER_H

#include <QList>
#include <QMap>
#include <QSharedPointer>
#include <qtypes.h>
#include "InstructionEditor/instruction.h"

namespace InstructionEditor {

    struct InstructionDefinition{
        quint8 opcode;
        InstructionType type;
        QString format;
    };

    typedef QMap<QString, InstructionDefinition> InstructionSet;

    // list of instructions accepted, constant for now, make dynamic later
    inline InstructionSet DEFAULT_INSTRUCTION_SET = {
        { "NOP",    {0, InstructionType::R, ""} },
        { "ADD",    {1, InstructionType::R, "r1, r2, r3"} },
        { "LDH",    {2, InstructionType::I, "r2, i(r1)"} },
        { "ADDI",   {3, InstructionType::I, "r1, i, r2"} },
        { "JUMP",   {4, InstructionType::J, "j"} },
        { "BRZ",    {5, InstructionType::I, "r1, j"} }
    };

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

        ParseStatus(ErrorSeverity severity, const QString &msg="", const Token &token = {"", -1, -1});

        static ParseStatus done(const QString &msg="", const Token &token = {"", -1, -1});
        static ParseStatus warning(const QString &msg="", Token const &token = {"", -1, -1});
        static ParseStatus fail(const QString &msg="", Token const &token = {"", -1, -1});

        QString toString() const;
    };

    QDebug operator<<(QDebug dbg, const ParseStatus &status);

    struct ParseResult{
        QSharedPointer<Instruction> instruction;
        ParseStatus status;
    };

    class InstructionParser
    {
    private:
        InstructionSet instructionSet;
        QList<char> separatorTokens;
        QMap<QString, quint32> labelMap;

        qsizetype currentLineNumber;
        qsizetype currentCharIndex;
        QString charBuffer;
        TokenList tokenBuffer;

        void flushCharBuffer();
        TokenList tokenize(const qsizetype lineNumber, const QString &line);
        ParseStatus mapTokens(const TokenList &formatTokens, const TokenList &argumentTokens, QMap<QString, Token> &tokenMappings) const;
        ParseResult parseRType(const QMap<QString, Token> &tokenMappings, const InstructionDefinition &definition) const;
        ParseResult parseIType(const QMap<QString, Token> &tokenMappings, const InstructionDefinition &definition) const;
        ParseResult parseJType(const QMap<QString, Token> &tokenMappings, const InstructionDefinition &definition) const;
    public:

        InstructionParser();
        bool addInstruction(const QString &mnemonic, const InstructionType type, const QString &format);
        bool removeInstruction(const QString &mnemonic);
        bool addLabel(const QString &label, const quint32 address);
        bool removeLabel(const QString &label);
        // bool isValidLabel(const QString &label);
        // quint32 getLabelAddress(const QString &label, bool *okptr);
        ParseResult parseLine(const qsizetype lineNumber, const QString &instruction);
    };

} // namespace InstructionEditor

#endif // INSTRUCTIONPARSER_H
