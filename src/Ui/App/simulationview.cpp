#include "simulationview.h"
#include "ui_simulationview.h"

SimulationView::SimulationView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SimulationView)
{
    ui->setupUi(this);

    connect(ui->clockButton, &QPushButton::clicked,
            this, &SimulationView::clockClicked);

    connect(ui->rewindButton, &QPushButton::clicked,
            this, &SimulationView::rewindClicked);

    connect(ui->resetButton, &QPushButton::clicked,
            this, &SimulationView::resetClicked);
}

SimulationView::~SimulationView()
{
    delete ui;
}

QString getRegString(uint32_t value){
    return QString("0x%1").arg(value, 8, 16, QLatin1Char('0'));
}

void SimulationView::updatePC(uint32_t pc)
{
    ui->pcValueLabel->setText(
        QString("0x%1").arg(pc, 8, 16, QLatin1Char('0'))
        );
}

void SimulationView::updateUAR(uint32_t uar)
{
    ui->uarValueLabel->setText(
        QString::number(uar)
        );
}

void SimulationView::updateState(const Machine::MachineState * state)
{
    ui->pcValueLabel->setText(getRegString(state->getPC()));
    ui->irValueLabel->setText(getRegString(state->getIR()));
    ui->aValueLabel->setText(getRegString(state->getA()));
    ui->bValueLabel->setText(getRegString(state->getB()));
    ui->cValueLabel->setText(getRegString(state->getC()));
}


