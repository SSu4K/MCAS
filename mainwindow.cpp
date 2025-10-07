#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "microcodeeditorwidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    auto* editor = new MicrocodeEditorWidget(this);
    setCentralWidget(editor);
    qDebug("Started window");
}

MainWindow::~MainWindow()
{
    delete ui;
}
