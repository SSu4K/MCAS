#ifndef EXECUTIONWORKER_H
#define EXECUTIONWORKER_H

#include "simLib_export.h"

#include "executionengine.h"

namespace Sim {

class SIMLIB_API ExecutionWorker : public QObject
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

    bool executeOneInstruction(QString &err);

    void runContinuous(double hz);

    const Machine::MachineState *getMachineState();

    uint32_t currentPC() const;
    uint32_t currentUAR() const;
    void setFrequency(double hz);

signals:
    void microStepDone(const Effects &fx);
    void instructionDone();
    void halted(const QString &reason);
    void stateChanged();

private slots:
    void onClockTick();

public slots:
    bool reset();
    bool stepMicro();
    bool rewindMicro();
    bool stepInstr();
    bool rewindInstruction();

    void run();
    void stop();

private:
    bool executeOneMicro(QString &err);
    Machine::MachineState *state = nullptr;
    ExecutionEngine *engine = nullptr;

    QTimer clock;
    Mode mode = Mode::Stopped;
    double frequency = 1;

    std::vector<Effects> history;
};


}
#endif // EXECUTIONWORKER_H
