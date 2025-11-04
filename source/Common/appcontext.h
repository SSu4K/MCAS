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

// Forward declarations until actual implementations are present
class MicrocodeModel;
class JumpTableModel;
class MemoryModel;

class AppContext : public QObject
{
    Q_OBJECT

public:
    enum class Theme {System, Light, Dark};
    Q_ENUM(Theme)
    enum class Language { System, English, Polish };
    Q_ENUM(Language)

    static AppContext* instance();

    MicrocodeModel* microcodeModel() const { return m_microcodeModel; }
    JumpTableModel* jumpTableModel() const { return m_jumpTableModel; }

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

    // Temporary until actual implementations are present
    MicrocodeModel* m_microcodeModel = nullptr;
    JumpTableModel* m_jumpTableModel = nullptr;

    std::unique_ptr<QSettings> m_settings;
    QTranslator m_translator;

    QString m_appVersion;
    QString m_buildType;
    QString m_platform;
    QString m_compiler;

    void loadTranslator(Language lang);
};
#endif // APPCONTEXT_H
