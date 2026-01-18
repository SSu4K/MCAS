#include "executionworker.h"

using namespace Sim;

using Machine::MachineState;

void revertEffects(MachineState &state, const Effects &fx)
{
    for (const auto &r : fx.regs) {
        switch(r.index){
            case Machine::SpecRegIndex::PC: state.setPC(r.oldValue); break;
            case Machine::SpecRegIndex::IR: state.setIR(r.oldValue); break;
            case Machine::SpecRegIndex::MAR: state.setMAR(r.oldValue); break;
            case Machine::SpecRegIndex::MDR: state.setMDR(r.oldValue); break;
            case Machine::SpecRegIndex::A: state.setA(r.oldValue); break;
            case Machine::SpecRegIndex::B: state.setB(r.oldValue); break;
            case Machine::SpecRegIndex::C: state.setC(r.oldValue); break;
            case Machine::SpecRegIndex::TEMP: state.setTemp(r.oldValue); break;
            default: state.setReg(r.index, r.oldValue);
        }
    }

    for (const auto &m : fx.memWrites) {
        state.storeByte(m.address, m.oldByte);
    }
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

    if (!engine->stepMicro(fx, err)) {
        emit halted(err);
        return false;
    }

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
    engine->setMicroAddress(rec.oldUAR);

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

