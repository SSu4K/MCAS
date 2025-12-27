#ifndef TESTEXECUTIONENGINE_H
#define TESTEXECUTIONENGINE_H

#include <QtTest/QTest>

class TestExecutionEngine : public QObject {
    Q_OBJECT

private slots:
    //void micro_alu_sets_last_result();
    void micro_jump_eq_uses_alu_result();
    // void fetch_increments_pc_and_sets_ir();
    // void decode_rtype_sets_ab();
    // void wf2_writes_correct_register();
};

#endif // TESTEXECUTIONENGINE_H
