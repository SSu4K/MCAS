#include "appcontext.h"

QPointer<AppContext> AppContext::s_instance = nullptr;

AppContext::AppContext(QObject* parent)
    : QObject(parent)
{
    m_settings = std::make_unique<QSettings>("SzymonSudak", "MCAS");
    m_microcodeModel = nullptr;
    m_jumpTableModel = nullptr;
}

AppContext* AppContext::instance()
{
    if (!s_instance)
        s_instance = new AppContext();
    return s_instance;
}
