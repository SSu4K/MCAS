#ifndef MICROCODE_INSTRUCTION_H
#define MICROCODE_INSTRUCTION_H

namespace Microcode {

    namespace InstructionField{
        enum {address=0, label, alu, s1, s2, dest, extir, constant, jcond, adr, mem, madr, mdest, regs, fieldCount};
    };

        static const uint32_t NO_JUMP = -1;

    class Instruction{
    public:

        uint32_t jumpAddress = NO_JUMP;
        uint32_t constantValue = 0;
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
        bool isEmpty() const;
    };
} // namespace Microcode

#endif // INSTRUCTION_H
