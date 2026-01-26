#ifndef CONFIGWINDOW_H
#define CONFIGWINDOW_H

#include "Common/editorwindow.h"
#include "instructionseteditorwidget.h"

namespace Models{
    class InstructionSetModel;
}

namespace Ui{

class ConfigWindow : public EditorWindow
{
    Q_OBJECT
public:
    explicit ConfigWindow(Models::InstructionSetModel* instructionSetModel, QWidget* parent = nullptr);

public slots:
    void open();

protected:
    QString windowTitle() const override { return tr("Configuration"); }
    QString openFilePrompt() const override { return tr("Open Config File"); }
    QString saveAsFilePrompt() const override { return tr("Save Config File As"); }
    QString defaultFilename() const override { return "config.cfg"; }
    QString fileFilterString() const override { return "Config Files (*.cfg)"; }

    bool serializeToFile(QFile& file) const override;
    bool serializeFromFile(QFile& file) override;
    void clearData() override;

    void createCustomMenu() override;

private:
    Models::InstructionSetModel* instructionSetModel;
    InstructionSetEditorWidget instructionSetEditorWidget;
    // widgets
};

}

#endif // CONFIGWINDOW_H
