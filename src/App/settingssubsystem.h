#ifndef SETTINGSSUBSYSTEM_H
#define SETTINGSSUBSYSTEM_H

#include <QObject>
#include <QSettings>

class SettingsSubsystem : public QObject
{
    Q_OBJECT
public:

    explicit SettingsSubsystem(QObject* parent = nullptr);

    QString currentTheme() const;
    QString language() const;

public slots:
    void setTheme(const QString& theme);
    void setLanguage(const QString& lang);

signals:
    void themeChanged(const QString&);
    void languageChanged(const QString&);

private:
    QSettings m_settings;
};

#endif // SETTINGSSUBSYSTEM_H
