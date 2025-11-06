#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <QList>
#include <qtypes.h>

namespace InstructionEditor{

    // const variables for now, needed to encode instructions into binary
    // in future should be modifiable with settings screen.
    // then the sizes and masks need to dynamically adjusted

    const quint8 OPCODE_COUNT = 64;
    const quint8 OPCODE_SIZE = 6;       // log64 = 6 -> 6 bits needed for encoding
    const quint32 OPCODE_MASK = 0x3F;   // mask = b00111111 = 0x3F

    const quint8 REGISTER_COUNT = 32;
    const quint8 REGISTER_SIZE = 5;     // log32 = 5 -> 5 bits needed for encoding
    const quint32 REGISTER_MASK = 0x1F; // mask = b00011111 = 0x1F

    // R-type
    // OPCODE_SIZE + FORMAL_COUNT * REGISTER_SIZE <= 32
    const quint8 R_FORMAL_COUNT = 5;
    const quint8 R_UNUSED_SIZE = 32 - (OPCODE_SIZE + R_FORMAL_COUNT*REGISTER_SIZE); // == 1

    // I-type
    // OPCODE_SIZE + 2 * REGISTER_SIZE + I_IMMEDIATE_SIZE <= 32
    const quint8 I_IMMEDIATE_SIZE = 16;
    const quint32 I_IMMEDIATE_MASK = 0xFFFF;
    const quint8 I_UNUSED_SIZE = 32 - (OPCODE_SIZE + 2 * REGISTER_SIZE + I_IMMEDIATE_SIZE);

    // J-type
    // OPCODE_SIZE + J_IMMEDIATE_SIZE <= 32
    const quint8 J_IMMEDIATE_SIZE = 26;
    const quint32 J_IMMEDIATE_MASK = 0x03FFFFFF;
    const quint8 J_UNUSED_SIZE = 32 - (OPCODE_SIZE + J_IMMEDIATE_SIZE);

    class Instruction{

    public:
        explicit Instruction() = default;
        quint8 opcode;
        virtual quint32 encode();
    };

    class RType: public Instruction{

    public:
        QList<quint8> formals;

        explicit RType();
        explicit RType(quint8 opcode, QList<quint8> formals);
        quint32 encode() override;
        static RType decode(quint32 instruction);
    };

    class IType: public Instruction{

    public:
        quint8 sourceRegister;
        quint8 destinationRegister;
        quint16 immediate;

        explicit IType() = default;
        explicit IType(quint8 opcode, quint8 source, quint8 destination, quint16 immediate);
        quint32 encode() override;
        static IType decode(quint32 instruction);
    };

    class JType: public Instruction{

    public:
        quint32 immediate;

        explicit JType() = default;
        explicit JType(quint8 opcode, quint32 immediate);
        quint32 encode() override;
        static JType decode(quint32 instruction);
    };

}

#endif // INSTRUCTION_H
