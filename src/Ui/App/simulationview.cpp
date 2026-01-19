#include <QDoubleValidator>

#include "simulationview.h"
#include "ui_simulationview.h"

static void initRegisterDisplay(ValueDisplayWidget* widget, const QString &label){
    widget->setLabel(label);
    widget->setBitWidth(32);
    widget->setDisplayBase(ValueDisplayWidget::Hexadecimal);
}

SimulationView::SimulationView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SimulationView)
{
    ui->setupUi(this);

    // Init clock display
    ui->clockDisplayWidget->setLabel("Clock:");
    ui->clockDisplayWidget->setBitWidth(32);
     ui->clockDisplayWidget->setDisplayBase(ValueDisplayWidget::UnsignedDecimal);

    // Init special register displays
    initRegisterDisplay(ui->uarDisplayWidget, "uAR:");
    initRegisterDisplay(ui->pcDisplayWidget, "PC:");
    initRegisterDisplay(ui->irDisplayWidget, "IR:");
    initRegisterDisplay(ui->aDisplayWidget, "A:");
    initRegisterDisplay(ui->bDisplayWidget, "B:");
    initRegisterDisplay(ui->cDisplayWidget, "C:");
    initRegisterDisplay(ui->marDisplayWidget, "MAR:");
    initRegisterDisplay(ui->mdrDisplayWidget, "MDR:");

    // Init gereral registers displays
    initRegisterList(32);

    // Init clock frequency edit
    auto* validator = new QDoubleValidator(0.001, 1e6, 3, ui->hzEdit);
    // validator->setNotation(QDoubleValidator::StandardNotation);
    // ui->hzEdit->setValidator(validator);
    validator->setLocale(QLocale::c());
    // ui->hzEdit->setText("1.0");

    // Connect buttons
    connect(ui->resetButton, &QPushButton::clicked,
            this, &SimulationView::resetClicked);

    connect(ui->hzEdit, &QDoubleSpinBox::editingFinished,
            this, &SimulationView::onClockFrequencyEdited);

    connect(ui->clockButton, &QPushButton::clicked,
            this, &SimulationView::clockClicked);
    connect(ui->rewindButton, &QPushButton::clicked,
            this, &SimulationView::rewindClicked);

    connect(ui->stepInstrButton, &QPushButton::clicked,
            this, &SimulationView::stepInstrClicked);
    connect(ui->rewindInstrButton, &QPushButton::clicked,
            this, &SimulationView::rewindInstrClicked);

    connect(ui->runButton, &QPushButton::clicked,
            this, &SimulationView::runClicked);
    connect(ui->stopButton, &QPushButton::clicked,
            this, &SimulationView::stopClicked);
}

SimulationView::~SimulationView()
{
    delete ui;
}

void SimulationView::initRegisterList(int registerCount)
{
    auto* container = new QWidget(this);
    auto* layout = new QVBoxLayout(container);

    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(2);

    for (int i = 0; i < registerCount; ++i)
    {
        auto* w = new ValueDisplayWidget(container);

        w->setLabel(QString("R%1:").arg(i));
        w->setBitWidth(32);
        w->setDisplayBase(ValueDisplayWidget::Hexadecimal);

        layout->addWidget(w);
        m_gpRegisters.push_back(w);
    }

    layout->addStretch();

    ui->registerScrollArea->setWidget(container);
    ui->registerScrollArea->setWidgetResizable(true);
}


QString getRegString(uint32_t value){
    return QString("0x%1").arg(value, 8, 16, QLatin1Char('0'));
}

void SimulationView::updatePC(uint32_t pc)
{

}

void SimulationView::updateUAR(uint32_t uar)
{
    ui->uarDisplayWidget->setValue(uar);
}

void SimulationView::updateState(const Machine::MachineState * state)
{
    ui->clockDisplayWidget->setValue(state->getClock());
    ui->pcDisplayWidget->setValue(state->getPC());
    ui->irDisplayWidget->setValue(state->getIR());
    ui->aDisplayWidget->setValue(state->getA());
    ui->bDisplayWidget->setValue(state->getB());
    ui->cDisplayWidget->setValue(state->getC());
    ui->marDisplayWidget->setValue(state->getMAR());
    ui->mdrDisplayWidget->setValue(state->getMDR());

    for (size_t i = 0; i < m_gpRegisters.size(); ++i)
    {
        auto* w = m_gpRegisters[i];
        auto value = state->getReg(i);

        w->setValue(value);
    }
}

void SimulationView::setRunning(bool running){
    ui->clockButton->setEnabled(!running);
    ui->rewindButton->setEnabled(!running);
    ui->stepInstrButton->setEnabled(!running);
    ui->rewindInstrButton->setEnabled(!running);

    ui->runButton->setEnabled(!running);
    ui->stopButton->setEnabled(running);
}

void SimulationView::onClockFrequencyEdited()
{
    bool ok = false;
    double hz = ui->hzEdit->text().toDouble(&ok);

    if (!ok || hz <= 0.0)
        return;

    emit clockFrequencyChanged(hz);

    //ui->hzEdit->setText(QString::number(hz, 'f', 3));
    ui->hzEdit->clearFocus();
    //ui->hzEdit->deselect();
}


