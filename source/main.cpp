#include "mcasapp.h"

int main(int argc, char *argv[])
{
    MCASApp app(argc, argv);

    qDebug() << "App version:" << AppContext::instance()->appVersion();
    qDebug() << "Build type:"  << AppContext::instance()->buildType();
    qDebug() << "Platform:"    << AppContext::instance()->platform();
    qDebug() << "Compiler:"    << AppContext::instance()->compiler();
    qDebug() << "Qt version:"  << AppContext::instance()->qtVersion();
    qDebug() << "Runtime OS:"  << AppContext::instance()->runtimeOS();

    app.context()->setTheme(AppContext::Theme::System);
    app.initPalette();
    return app.exec();
}


