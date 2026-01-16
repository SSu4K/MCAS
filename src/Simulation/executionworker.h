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

    void setMachineState(Machine::MachineState *state);
    void setExecutionEngine(ExecutionEngine *engine);

    bool stepInstruction(QString &err);

    void runContinuous(double hz);
    void stop();

    bool rewindMicro();
    bool rewindInstruction();

    const Machine::MachineState *getMachineState();

    uint32_t currentPC() const;
    uint32_t currentUAR() const;

signals:
    void microStepDone(const Effects &fx);
    void instructionDone();
    void halted(QString reason);
    void stateChanged();

private slots:
    void onClockTick();

public slots:
    bool stepMicro();
    bool reset();

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
