#ifndef SIMULATIONVIEW_H
#define SIMULATIONVIEW_H

#include <QWidget>
#include "Machine/machinestate.h"

class ValueDisplayWidget;

namespace Ui {
class SimulationView;
}

class SimulationView : public QWidget
{
    Q_OBJECT

public:
    explicit SimulationView(QWidget *parent = nullptr);
    ~SimulationView();

signals:
    void clockClicked();
    void rewindClicked();
    void resetClicked();

public slots:
    void updatePC(uint32_t pc);
    void updateUAR(uint32_t uar);
    void updateState(const Machine::MachineState * state);

private:
    Ui::SimulationView *ui;
    std::vector<ValueDisplayWidget*> m_gpRegisters;

    void initRegisterList(int registerCount);
};

#endif // SIMULATIONVIEW_H
