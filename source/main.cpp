#include "mcasapp.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QStyleFactory>


QPalette createDarkPalette() {
    QPalette darkPalette;

    darkPalette.setColor(QPalette::Window, QColor(37, 37, 38));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(25, 25, 25));
    darkPalette.setColor(QPalette::AlternateBase, QColor(45, 45, 45));
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Button, QColor(45, 45, 45));
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::BrightText, Qt::red);

    darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::HighlightedText, Qt::white);

    darkPalette.setColor(QPalette::Disabled, QPalette::Text, QColor(120, 120, 120));
    darkPalette.setColor(QPalette::Disabled, QPalette::ButtonText, QColor(120, 120, 120));

    return darkPalette;
}

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


