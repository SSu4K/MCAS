#ifndef INSTRUCTIONPARSER_H
#define INSTRUCTIONPARSER_H

#include <QList>
#include <QMap>
#include <qtypes.h>
#include "InstructionEditor/instruction.h"

namespace InstructionEditor {

    // list of instructions accepted, constant for now, make dynamic later
    enum class InstructionType{R, I, J};

    struct IntructionDefinition{
        quint8 index;
        InstructionType type;
        QString format;
    };

    typedef QMap<QString, IntructionDefinition> InstructionSet;

    inline InstructionSet DEFAULT_INSTRUCTION_SET = {
        { "NOP",    {0, InstructionType::R, ""} },
        { "ADD",    {1, InstructionType::R, "r1, r2, r3"} },
        { "LDH",    {2, InstructionType::I, "r2, i(r1)"} },
        { "ADDI",   {3, InstructionType::I, "r1, i, r2"} },
        { "JUMP",   {4, InstructionType::J, "j"} },
        { "BRZ",    {5, InstructionType::I, "r1, j"} }
    };

    //QStringList tokens = {"r1"}
    inline QList<char> DEFAULT_SEPARATOR_TOKENS = {',', '(', ')'};

    class InstructionParser
    {
    private:
        InstructionSet instructionSet;
        QList<char> separatorTokens;

        QString charBuffer;
        QStringList tokenBuffer;

        void flushCharBuffer();
        QStringList tokenize(const QString &line);
        //QMap<QString, QString> mapTokens(const QStringList &formatTokens, const QStringList &argumentTokens) const;
    public:
        QVariant parse(const QString &instruction);
        InstructionParser();
    };

} // namespace InstructionEditor

#endif // INSTRUCTIONPARSER_H
