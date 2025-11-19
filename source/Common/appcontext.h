#ifndef APPCONTEXT_H
#define APPCONTEXT_H

#include <QObject>
#include <QPointer>
#include <QSettings>
#include <memory>
#include <QString>
#include <QSysInfo>
#include <QVersionNumber>
#include <qtranslator.h>

#include "MicrocodeEditor/microcodedata.h"
#include "MicrocodeEditor/jumptabledata.h"
#include "MemoryEditor/memorydata.h"
#include "MemoryEditor/memorymodel.h"
#include "InstructionEditor/instructiondata.h"
#include "Assembler/labeldata.h"
#include "Assembler/instructiondefinition.h"

using namespace MicrocodeEditor;
using namespace MemoryEditor;
using namespace InstructionEditor;
using namespace Assembly;

class SharedData: public QObject{
    Q_OBJECT
public:
    explicit SharedData(QObject* parent = nullptr);

    std::shared_ptr<MemoryModel> memory() const { return m_memory; }
    std::shared_ptr<InstructionData> instructions() const { return m_instructions; }
    std::shared_ptr<MicrocodeData> microcode() const { return m_microcode; }
    std::shared_ptr<JumpTableData> jumptable() const { return m_jumptable; }
    std::shared_ptr<LabelData> labels() const { return m_labelData; }
    std::shared_ptr<InstructionSet> instructionSet() const { return m_instructionSet;}

signals:
    void memoryUpdated();
    void instructionsUpdated();
    void microcodeUpdated();
    void configChanged();

private:
    std::shared_ptr<MemoryModel> m_memory;
    std::shared_ptr<InstructionData> m_instructions;
    std::shared_ptr<MicrocodeData> m_microcode;
    std::shared_ptr<JumpTableData> m_jumptable;
    std::shared_ptr<LabelData> m_labelData;
    std::shared_ptr<InstructionSet> m_instructionSet;
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
