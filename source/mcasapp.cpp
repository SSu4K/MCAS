#include "mcasapp.h"

MCASApp::MCASApp(int &argc, char **argv)
    : QApplication(argc, argv)
{
    m_context = AppContext::instance();
}
