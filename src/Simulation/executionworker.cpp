#include "executionworker.h"

using namespace Sim;

using Machine::MachineState;

void revertEffects(MachineState &state, const Effects &fx)
{
    for (const auto &r : fx.regs) {
        state.setReg(r.index, r.oldValue);
    }

    for (const auto &m : fx.memWrites) {
        state.storeByte(m.address, m.oldByte);
    }

    state.setPC(fx.oldPc);
}

ExecutionWorker::ExecutionWorker(QObject *parent): QObject(parent){}

bool ExecutionWorker::executeOneMicro(QString &err)
{
    Effects fx;

    fx.oldPc = state->getPC();

    if (!engine->stepMicro(fx, err)) {
        emit halted(err);
        return false;
    }

    fx.newPc = state->getPC();

    history.push_back(fx);

    emit microStepDone(fx);
    emit stateChanged();
    return true;
}

bool ExecutionWorker::stepMicro(QString &err){
    return executeOneMicro(err);
}

void ExecutionWorker::runContinuous(double hz)
{
    mode = Mode::Continuous;
    clock.start(static_cast<int>(1000.0 / hz));
}

void ExecutionWorker::onClockTick()
{
    QString err;
    if (!stepMicro(err)) {
        stop();
    }
}

void ExecutionWorker::stop()
{
    clock.stop();
    mode = Mode::Stopped;
}

bool ExecutionWorker::rewindMicro()
{
    if (history.empty())
        return false;

    const auto rec = history.back();
    history.pop_back();

    revertEffects(*state, rec);

    emit stateChanged();
    return true;
}

