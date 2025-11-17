#include "appcontext.h"
#include <QCoreApplication>

QPointer<AppContext> AppContext::s_instance = nullptr;

SharedData::SharedData(QObject* parent)
    : //QObject(parent),
    m_memory(std::make_shared<MemoryModel>()),
    m_instructions(std::make_shared<InstructionData>()),
    m_microcode(std::make_shared<MicrocodeData>()),
    m_jumptable(std::make_shared<JumpTableData>())
{}

AppContext::AppContext(QObject* parent)
    : QObject(parent), m_sharedData(std::make_shared<SharedData>())
{
    m_settings = std::make_unique<QSettings>("SzymonSudak", "MCAS");
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

AppContext::Language AppContext::currentLanguage() const{
    QString value = m_settings->value("ui/language", "System").toString();
    if (value == "English")  return Language::English;
    if (value == "Polish") return Language::Polish;
    return Language::System;
}

void AppContext::setLanguage(AppContext::Language lang){
    Language current = currentLanguage();
    if(current == lang){
        return;
    }

    QString value;
    switch (lang) {
        case Language::English:  value = "English"; break;
        case Language::Polish: value = "Polish"; break;
        case Language::System: value = "System"; break;
    }

    qApp->removeTranslator(&m_translator);
    m_settings->setValue("ui/language", value);
    loadTranslator(lang);

    emit languageChanged();
}

void AppContext::initLanguage(){
    Language current = currentLanguage();
    loadTranslator(current);
}

void AppContext::loadTranslator(Language lang){
    QString qmPath;
    switch (lang) {
        case Language::Polish:  qmPath = ":/i18n/MCAS_pl.qm"; break;
        case Language::English: qmPath = ":/i18n/MCAS_en.qm"; break;
        default: break;
    }

    if (m_translator.load(qmPath)){
        qApp->installTranslator(&m_translator);
        qDebug() << "Loding translator" << qmPath << "success!";
    }
    else{
        qDebug() << "Loding translator" << qmPath << "failed!";
    }
}
