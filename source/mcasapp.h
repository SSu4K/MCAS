#ifndef MCASAPP_H
#define MCASAPP_H

#include <QApplication>
#include <QStyleFactory>
#include <QPalette>
#include <QTranslator>

#include "appcontext.h"
#include "mainwindow.h"

class MCASApp : public QApplication
{
    Q_OBJECT
public:
    MCASApp(int &argc, char **argv);
    ~MCASApp() override = default;

    AppContext* context() { return m_context; }
    MainWindow* mainWindow() const { return m_mainWindow; }

    // App-wide setup
    void initTranslations();
    void initPalette();
    void initMainWindow();


private:
    AppContext *m_context = nullptr;
    QTranslator m_translator;
    MainWindow *m_mainWindow = nullptr;
};
#endif // MCASAPP_H
