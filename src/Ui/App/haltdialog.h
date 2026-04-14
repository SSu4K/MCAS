#ifndef HALTDIALOG_H
#define HALTDIALOG_H

#include <QDialog>

namespace Ui {
class HaltDialog;
}

class HaltDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HaltDialog(QWidget *parent = nullptr);
    void setReason(const QString &reason);
    ~HaltDialog();

private:
    Ui::HaltDialog *ui;
};

#endif // HALTDIALOG_H
