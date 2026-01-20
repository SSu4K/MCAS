#ifndef TESTEXECUTIONENGINE_H
#define TESTEXECUTIONENGINE_H

#include <QtTest/QTest>
#include "Machine/machinestate.h"
#include "Assembly/instructiondefinition.h"

class TestExecutionEngine : public QObject {
    Q_OBJECT

    Machine::MachineConfig machineConfig;
    Assembly::InstructionSet instructionSet;

public:
    TestExecutionEngine(QObject* parent = nullptr);

private slots:

    // basic uar incrementing tests
    void uar_advances_without_jump();
    void true_jump_sets_uar();

    // regs op tests
    void rr_reads_formals();
    void wf_writes_c_into_formal();

    // ALU tests
    void alu_add_sets_C();

    void extir_byte_sign_extends();

    void memory_write_stores_word();

    // Microcode run tests
    void basic_fetch();
};


#endif // TESTEXECUTIONENGINE_H
