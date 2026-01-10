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
    void micro_alu_sets_result();
    void micro_jump_eq_uses_alu_result();
    void fetch_increments_pc_and_sets_ir();
    void rr_loads_ab_from_formals();
    // void wf2_writes_correct_register();
};

#endif // TESTEXECUTIONENGINE_H
