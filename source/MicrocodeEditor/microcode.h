#ifndef MICROCODE_H
#define MICROCODE_H

#include <QString>
#include <QList>
#include <QStringList>
#include <QMap>
#include <QVariant>

namespace MicrocodeEditor{
    namespace InstructionField{
        enum {address=0, label, alu, s1, s2, dest, extir, constant, jcond, adr, mem, madr, mdest, regs, fieldCount};
    };

    const QString emptyOptionString = "";
    const QStringList aluOptions    = {emptyOptionString, "ADD", "SUB", "RSUB", "AND", "OR", "XOR", "SSL", "SRA", "S1", "S2", "S2S1", "MUL", "DIV"};
    const QStringList s1Options     = {emptyOptionString, "A", "Const", "PC", "MAR", "MDR", "IR"};
    const QStringList s2Options     = {emptyOptionString, "B", "Const", "PC", "MAR", "MDR", "IR"};
    const QStringList destOptions   = {emptyOptionString, "C", "PC", "MAR", "MDR"};
    const QStringList jcondOptions  = {emptyOptionString, "True", "EQ", "NE", "LT", "GT", "LE", "GE", "MBusy", "Jump1", "Jump2"};
    const QStringList extirOptions  = {emptyOptionString, "Byte", "Half", "Word"};
    const QStringList memOptions    = {emptyOptionString, "RB", "RH", "RW", "WB", "WH", "WW"};
    const QStringList madrOptions   = {emptyOptionString, "MAR", "PC"};
    const QStringList mdestOptions  = {emptyOptionString, "MAR", "PC"};
    const QStringList regsOptions   = {
                                        emptyOptionString, "RR",
                                        "RAF1", "RAF2", "RAF3",
                                        "RBF1", "RBF2", "RBF3",
                                        "WF1", "WF2", "WF3",
                                        "RAA1", "RAA2", "RAA3",
                                        "RBA1", "RBA2", "RBA3",
                                        "WA1", "WA2", "WA3"
                                        };

    class Instruction{
    public:
        static const QMap<size_t, QString> fieldNames;
        static const QMap<size_t, QStringList> validStringValues;

        QString address;
        QString label;
        QString alu;
        QString s1;
        QString s2;
        QString dest;
        QString extir;
        QString constant;
        QString jcond;
        QString adr;
        QString mem;
        QString madr;
        QString mdest;
        QString regs;

        static const QStringList getValidStringValues(const size_t field);
        static const bool isValidStringValue(const size_t field, const QString &string, Qt::CaseSensitivity cs = Qt::CaseSensitive);
        static const QString matchValidFieldValue(const size_t field, const QString &string, bool * okptr);

        Instruction();

        QVariant getFieldValue(const size_t field) const;
        bool setFieldValue(const size_t field, const QVariant &value);
    };

    class Microcode{
    public:
        QList<Instruction> instructions;
        Microcode();
    };

}
#endif // MICROCODE_H
