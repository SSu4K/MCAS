#include "settingssubsystem.h"

SettingsSubsystem::SettingsSubsystem(QObject* parent)
    : QObject(parent)
    , m_settings("SzymonSudak", "MCAS")
{
}

QString SettingsSubsystem::currentTheme() const
{
    return m_settings.value("ui/theme", "light").toString();
}

QString SettingsSubsystem::language() const
{
    return m_settings.value("ui/language", "en").toString();
}

void SettingsSubsystem::setTheme(const QString& theme)
{
    qDebug() << "settings: setTheme:" << theme;
    if (theme == currentTheme())
        return;

    m_settings.setValue("ui/theme", theme);
    emit themeChanged(theme);
}

void SettingsSubsystem::setLanguage(const QString& lang)
{
    if (lang == this->language())
        return;

    m_settings.setValue("ui/language", lang);
    emit languageChanged(lang);
}
