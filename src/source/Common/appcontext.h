#ifndef APPCONTEXT_H
#define APPCONTEXT_H

namespace MicrocodeEditor{
    class MicrocodeData;
    class JumpTableData;
}
namespace MemoryEditor{ class MemoryModel; }
namespace InstructionEditor{ class InstructionData; }
namespace Assembly{
    class LabelData;
    class InstructionSet;
}

class SharedData: public QObject{
    Q_OBJECT
public:
    explicit SharedData(QObject* parent = nullptr);

    std::shared_ptr<MemoryEditor::MemoryModel> memory() const { return m_memory; }
    std::shared_ptr<InstructionEditor::InstructionData> instructions() const { return m_instructions; }
    std::shared_ptr<MicrocodeEditor::MicrocodeData> microcode() const { return m_microcode; }
    std::shared_ptr<MicrocodeEditor::JumpTableData> jumptable() const { return m_jumptable; }
    std::shared_ptr<Assembly::LabelData> labels() const { return m_labelData; }
    std::shared_ptr<Assembly::InstructionSet> instructionSet() const { return m_instructionSet;}

signals:
    void memoryUpdated();
    void instructionsUpdated();
    void microcodeUpdated();
    void configChanged();

private:
    std::shared_ptr<MemoryEditor::MemoryModel> m_memory;
    std::shared_ptr<InstructionEditor::InstructionData> m_instructions;
    std::shared_ptr<MicrocodeEditor::MicrocodeData> m_microcode;
    std::shared_ptr<MicrocodeEditor::JumpTableData> m_jumptable;
    std::shared_ptr<Assembly::LabelData> m_labelData;
    std::shared_ptr<Assembly::InstructionSet> m_instructionSet;
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

    SharedData* sharedData() { return m_sharedData.get(); }
    QSettings* settings() const { return m_settings.get(); }

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

    std::shared_ptr<SharedData> m_sharedData;
    std::unique_ptr<QSettings> m_settings;
    QTranslator m_translator;

    QString m_appVersion;
    QString m_buildType;
    QString m_platform;
    QString m_compiler;

    void loadTranslator(Language lang);
};
#endif // APPCONTEXT_H
