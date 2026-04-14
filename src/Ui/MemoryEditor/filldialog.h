#ifndef FILLDIALOG_H
#define FILLDIALOG_H

#include <QDialog>

// forward declare
class QLineEdit;

namespace Ui {

    class FillDialog : public QDialog
    {
        Q_OBJECT
    public:
        explicit FillDialog(QWidget* parent = nullptr);

        int value() const;

    private slots:
        void validateInput();

    private:
        QLineEdit* m_input;
        QPushButton* m_okButton;
    };
}

#endif // FILLDIALOG_H
