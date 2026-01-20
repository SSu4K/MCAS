#include "haltdialog.h"
#include "ui_haltdialog.h"

HaltDialog::HaltDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::HaltDialog)
{
    ui->setupUi(this);
    connect(ui->buttonBox, &QDialogButtonBox::accepted,
            this, &QDialog::accept);
}

void HaltDialog::setReason(const QString &reason)
{
    ui->messageLabel->setText(reason);
}

HaltDialog::~HaltDialog(){
    delete ui;
}
