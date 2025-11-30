#ifndef MICROCODE_INSTRUCTION_H
#define MICROCODE_INSTRUCTION_H

namespace Microcode {

    namespace InstructionField{
        enum {address=0, label, alu, s1, s2, dest, extir, constant, jcond, adr, mem, madr, mdest, regs, fieldCount};
    };

    class Instruction{
    public:

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

        Instruction();

        QString getFieldValue(const size_t field) const;
        bool setFieldValue(const size_t field, const QString &value);
    };
} // namespace Microcode

#endif // INSTRUCTION_H
