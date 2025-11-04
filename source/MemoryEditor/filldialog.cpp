#include <QHBoxLayout>
#include <QFormLayout>
#include <QRegularExpressionValidator>
#include <QDialogButtonBox>
#include <QPushButton>
#include "filldialog.h"

using namespace MemoryEditor;

FillDialog::FillDialog(QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle(tr("Fill Memory"));

    auto* layout = new QVBoxLayout(this);

    auto* label = new QLabel(tr("Enter value:"), this);
    m_input = new QLineEdit(this);

    layout->addWidget(m_input);

    auto* buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    m_okButton = buttons->button(QDialogButtonBox::Ok);
    m_okButton->setEnabled(false);

    layout->addWidget(label);
    layout->addWidget(m_input);
    layout->addWidget(buttons);

    connect(m_input, &QLineEdit::textChanged, this, &FillDialog::validateInput);
    connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

void FillDialog::validateInput()
{
    QString text = m_input->text().trimmed();
    bool ok = false;
    int value = text.toInt(&ok, 16);
    if (!ok)
        value = text.toInt(&ok, 10);

    m_okButton->setEnabled(ok);
}

int FillDialog::value() const
{
    QString text = m_input->text().trimmed();
    bool ok = false;
    int value = text.toInt(&ok, 16);
    if (!ok)
        value = text.toInt(&ok, 10);

    return ok ? value : 0;
}

