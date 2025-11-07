#ifndef INSTRUCTIONPARSER_H
#define INSTRUCTIONPARSER_H

#include <QList>
#include <QMap>
#include <QSharedPointer>
#include <qtypes.h>
#include "InstructionEditor/instruction.h"

namespace InstructionEditor {

    struct IntructionDefinition{
        quint8 opcode;
        InstructionType type;
        QString format;
    };

    typedef QMap<QString, IntructionDefinition> InstructionSet;

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
        qsizetype address; // adress in string
        qsizetype index; // token id
    };

    typedef QList<Token> TokenList;

    enum class ErrorSeverity{
        Correct, Warning, Error
    };

    class ParseStatus{
    public:
        ErrorSeverity severity;
        QString msg;
        qsizetype tokenIndex;
        qsizetype charIndex;

        ParseStatus(ErrorSeverity severity, QString msg, qsizetype tokenIndex = -1, qsizetype charIndex = -1);

        static ParseStatus done(QString msg="", qsizetype tokenIndex = -1, qsizetype charIndex = -1);
        static ParseStatus warning(QString msg="", qsizetype tokenIndex = -1, qsizetype charIndex = -1);
        static ParseStatus fail(QString msg="", qsizetype tokenIndex = -1, qsizetype charIndex = -1);
    };

    struct ParseResult{
        QSharedPointer<Instruction> instruction;
        ParseStatus status;
    };

    class InstructionParser
    {
    private:
        InstructionSet instructionSet;
        QList<char> separatorTokens;

        qsizetype parseAddress;
        QString charBuffer;
        TokenList tokenBuffer;

        void flushCharBuffer();
        TokenList tokenize(const QString &line);
        ParseStatus mapTokens(const TokenList &formatTokens, const TokenList &argumentTokens, QMap<QString, Token> &tokenMappings) const;
    public:
        ParseResult parse(const QString &instruction);
        InstructionParser();
    };

} // namespace InstructionEditor

#endif // INSTRUCTIONPARSER_H
