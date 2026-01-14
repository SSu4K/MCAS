#ifndef EXECUTIONWORKER_H
#define EXECUTIONWORKER_H

#include "executionengine.h"

namespace Sim {

class ExecutionWorker : public QObject
{
    Q_OBJECT

public:
    enum class Mode {
        Stopped,
        Continuous
    };

    explicit ExecutionWorker(QObject *parent = nullptr);

    // Configuration
    void setMachineState(Machine::MachineState *state);
    void setExecutionEngine(ExecutionEngine *engine);

    // Execution
    bool stepMicro(QString &err);
    bool stepInstruction(QString &err);

    void runContinuous(double hz);
    void stop();

    // Rewind
    bool rewindMicro();
    bool rewindInstruction();

signals:
    void microStepDone(const Effects &fx);
    void instructionDone();
    void halted(QString reason);
    void stateChanged();

private slots:
    void onClockTick();

private:
    bool executeOneMicro(QString &err);

private:
    Machine::MachineState *state = nullptr;
    ExecutionEngine *engine = nullptr;

    QTimer clock;
    Mode mode = Mode::Stopped;

    std::vector<Effects> history;
};


}
#endif // EXECUTIONWORKER_H
