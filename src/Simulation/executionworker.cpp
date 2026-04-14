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

ExecutionWorker::ExecutionWorker(QObject *parent): QObject(parent){
    clock.setTimerType(Qt::PreciseTimer);
    clock.setSingleShot(false);

    connect(&clock, &QTimer::timeout,
            this, &ExecutionWorker::onClockTick);

}

void ExecutionWorker::setMachineState(Machine::MachineState *state){
    this->state = state;
}

void ExecutionWorker::setExecutionEngine(ExecutionEngine *engine){
    this->engine = engine;
}

bool ExecutionWorker::executeOneMicro(QString &err)
{
    auto haltStatus = engine->getHaltStatus();
    if(haltStatus.isHalted){
        mode = Mode::Stopped;
        engine->unhalt();
        return true;
    }


    Effects fx;

    if (!engine->stepMicro(fx, err)) {
        emit halted(err);
    }

    history.push_back(fx);

    emit microStepDone(fx);
    emit stateChanged();
    return true;
}

bool ExecutionWorker::executeOneInstruction(QString &err){

    if (mode != Mode::Stopped)
        return false;

    auto initialPC = state->getPC();
    while(initialPC == state->getPC()){
        if(!executeOneMicro(err)){
            return false;
        }
    }
    return true;
}

bool ExecutionWorker::stepMicro(){
    if (mode != Mode::Stopped)
        return false;

    QString err;
    return executeOneMicro(err);
}

bool ExecutionWorker::stepInstr(){
    if (mode != Mode::Stopped)
        return false;

    QString err;
    return executeOneInstruction(err);
}

bool ExecutionWorker::rewindMicro()
{
    if (mode != Mode::Stopped)
        return false;

    if (history.empty() || state->getClock() == 0)
        return false;

    const auto rec = history.back();
    history.pop_back();

    revertEffects(*state, rec);
    engine->setMicroAddress(rec.oldUAR);
    state->setClock(state->getClock()-1);

    emit stateChanged();
    return true;
}

bool ExecutionWorker::rewindInstruction(){
    if (mode != Mode::Stopped)
        return false;

    auto initialPC = state->getPC();
    while(initialPC == state->getPC()){
        if(!rewindMicro()){
            return false;
        }
    }
    return true;
}

bool ExecutionWorker::reset(){

    if (mode != Mode::Stopped)
        return false;

    bool success = engine->reset();
    emit stateChanged();
    return success;
}

void ExecutionWorker::runContinuous(double hz)
{
    if (mode != Mode::Stopped)
        return;

    mode = Mode::Continuous;

    int intervalMs = qMax(1, static_cast<int>(1000.0 / hz));
    clock.start(intervalMs);
}


void ExecutionWorker::onClockTick()
{
    if (mode != Mode::Continuous)
        return;

    QString err;
    if (!executeOneMicro(err)) {
        stop();
    }
}


void ExecutionWorker::run(){
    if (mode != Mode::Stopped)
        return;

    runContinuous(frequency);
}

void ExecutionWorker::stop()
{
    if (mode == Mode::Stopped)
        return;

    clock.stop();
    mode = Mode::Stopped;
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

void ExecutionWorker::setFrequency(double hz)
{
    if (hz <= 0.0)
        return;

    frequency = hz;

    int intervalMs = qMax(1, static_cast<int>(1000.0 / frequency));

    if (clock.isActive()) {
        clock.start(intervalMs);
    }
}

