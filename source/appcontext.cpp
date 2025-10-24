#include "appcontext.h"

QPointer<AppContext> AppContext::s_instance = nullptr;

AppContext::AppContext(QObject* parent)
    : QObject(parent)
{
    m_settings = std::make_unique<QSettings>("SzymonSudak", "MCAS");
    m_microcodeModel = nullptr;
    m_jumpTableModel = nullptr;
    m_appVersion = QStringLiteral(APP_VERSION);
    m_buildType  = QStringLiteral(BUILD_TYPE);
    m_platform   = QStringLiteral(BUILD_PLATFORM);
    m_compiler   = QStringLiteral(COMPILER_NAME);
}

AppContext* AppContext::instance()
{
    if (!s_instance)
        s_instance = new AppContext();
    return s_instance;
}

AppContext::Theme AppContext::currentTheme() const
{
    QString value = m_settings->value("ui/theme", "System").toString();
    if (value == "Dark")  return Theme::Dark;
    if (value == "Light") return Theme::Light;
    return Theme::System;
}

void AppContext::setTheme(AppContext::Theme theme)
{
    QString value;
    switch (theme) {
    case Theme::Dark:  value = "Dark"; break;
    case Theme::Light: value = "Light"; break;
    case Theme::System: value = "System"; break;
    }
    m_settings->setValue("ui/theme", value);
}
