#ifndef FILLDIALOG_H
#define FILLDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QLabel>

#include "Common/hexint.h"

namespace MemoryEditor {

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
