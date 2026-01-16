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

void ExecutionWorker::setMachineState(Machine::MachineState *state){
    this->state = state;
}

void ExecutionWorker::setExecutionEngine(ExecutionEngine *engine){
    this->engine = engine;
}

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

bool ExecutionWorker::stepMicro(){
    QString err;
    return executeOneMicro(err);
}

bool ExecutionWorker::reset(){
    bool success = engine->reset();
    emit stateChanged();
    return success;
}

void ExecutionWorker::runContinuous(double hz)
{
    mode = Mode::Continuous;
    clock.start(static_cast<int>(1000.0 / hz));
}

void ExecutionWorker::onClockTick()
{
    if (!stepMicro()) {
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

uint32_t ExecutionWorker::currentPC() const
{
    return state->getPC();
}

uint32_t ExecutionWorker::currentUAR() const
{
    return engine->currentMicroAddress();
}

const Machine::MachineState *ExecutionWorker::getMachineState(){
    return state;
}

