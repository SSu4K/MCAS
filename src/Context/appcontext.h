#ifndef APPCONTEXT_H
#define APPCONTEXT_H

#include "Assembler/instructiondefinition.h"
#include "Assembler/labeldata.h"
#include "Instruction/instructiondata.h"
#include "Microcode/jumptabledata.h"
#include "Microcode/microcodedata.h"
#include "machinestate.h"

class SharedData: public QObject{
    Q_OBJECT
public:
    explicit SharedData(QObject* parent = nullptr);

    InstructionEditor::InstructionData* instructions() { return &m_instructions; }
    MicrocodeEditor::MicrocodeData* microcode() { return &m_microcode; }
    MicrocodeEditor::JumpTableData* jumptable() { return &m_jumptable; }
    Assembly::LabelData* labels() { return &m_labelData; }
    Assembly::InstructionSet* instructionSet() { return &m_instructionSet;}

    MachineState* editorMachineState() {return &m_editorMachineState;}
    MachineConfig* machineConfig() {return &m_machineConfig;}

signals:
    void memoryUpdated();
    void instructionsUpdated();
    void microcodeUpdated();
    void configChanged();

private:
    MicrocodeEditor::MicrocodeData m_microcode;
    MicrocodeEditor::JumpTableData m_jumptable;
    Assembly::LabelData m_labelData;
    Assembly::InstructionSet m_instructionSet;

    MachineConfig m_machineConfig;
    MachineState m_editorMachineState;
    InstructionEditor::InstructionData m_instructions;
};

class AppContext : public QObject
{
    Q_OBJECT

public:
    enum class Theme {System, Light, Dark};
    Q_ENUM(Theme)
    enum class Language { System, English, Polish };
    Q_ENUM(Language)

    static AppContext* instance();
    static void freeInstance();

    SharedData* sharedData() { return &m_sharedData; }
    QSettings* settings() { return &m_settings; }

    QString appVersion() const { return m_appVersion; }
    QString buildType() const { return m_buildType; }
    QString platform() const { return m_platform; }
    QString compiler() const { return m_compiler; }
    QString qtVersion() const { return QT_VERSION_STR; }
    QString runtimeOS() const { return QSysInfo::prettyProductName(); }

    Theme currentTheme() const;
    void setTheme(Theme theme);

    Language currentLanguage() const;
    void setLanguage(Language lang);
    void initLanguage();

signals:
    void languageChanged();

private:
    explicit AppContext(QObject* parent = nullptr);
    ~AppContext() override = default;

    static QPointer<AppContext> s_instance;

    SharedData m_sharedData;
    QSettings m_settings;
    QTranslator m_translator;

    QString m_appVersion;
    QString m_buildType;
    QString m_platform;
    QString m_compiler;

    void loadTranslator(Language lang);
};
#endif // APPCONTEXT_H
