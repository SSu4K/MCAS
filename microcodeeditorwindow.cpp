#include "microcodeeditorwindow.h"

#include "microcodeeditorwindow.h"
#include "microcodeeditorwidget.h"
#include "microcodemodel.h"

#include <QMenuBar>
#include <QMenu>
#include <QFileDialog>
#include <QMessageBox>
#include <QCloseEvent>
#include <QFile>
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include "microcodeeditorwindow.h"
#include "microcodeeditorwidget.h"
#include "microcodemodel.h"

#include <QMenuBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>

MicrocodeEditorWindow::MicrocodeEditorWindow(QWidget* parent)
    : QMainWindow(parent)
{
    m_editorWidget = new MicrocodeEditorWidget(this);
    setCentralWidget(m_editorWidget);

    createMenus();
    setWindowTitle("Microcode Editor");
    resize(1000, 600);
}

void MicrocodeEditorWindow::createMenus()
{
    QMenu* fileMenu = menuBar()->addMenu(tr("&File"));

    // Modern, non-deprecated addAction() syntax
    fileMenu->addAction(tr("&New"), QKeySequence::New, this, &MicrocodeEditorWindow::newFile);
    fileMenu->addAction(tr("&Open..."), QKeySequence::Open, this, &MicrocodeEditorWindow::openFile);

    fileMenu->addSeparator();

    fileMenu->addAction(tr("&Save"), QKeySequence::Save, this, &MicrocodeEditorWindow::saveFile);
    fileMenu->addAction(tr("Save &As..."), QKeySequence::SaveAs, this, &MicrocodeEditorWindow::saveFileAs);

    fileMenu->addSeparator();

    fileMenu->addAction(tr("E&xit"), QKeySequence::Quit, this, &MicrocodeEditorWindow::exitApp);
}

void MicrocodeEditorWindow::newFile()
{
    if (!maybeSave())
        return;

    m_editorWidget->m_model->clear();
    m_currentFilePath.clear();
    setWindowTitle("Microcode Editor - [New]");
}

void MicrocodeEditorWindow::openFile()
{
    if (!maybeSave())
        return;

    QString filePath = QFileDialog::getOpenFileName(this, tr("Open Microcode File"),
                                                    "", tr("Microcode Files (*.txt *.mc *.tsv)"));
    if (filePath.isEmpty())
        return;

    if (loadFromFile(filePath)) {
        m_currentFilePath = filePath;
        setWindowTitle(QString("Microcode Editor - [%1]").arg(QFileInfo(filePath).fileName()));
    }
}

void MicrocodeEditorWindow::saveFile()
{
    if (m_currentFilePath.isEmpty()) {
        saveFileAs();
        return;
    }
    saveToFile(m_currentFilePath);
}

void MicrocodeEditorWindow::saveFileAs()
{
    QString filePath = QFileDialog::getSaveFileName(this, tr("Save Microcode File As"),
                                                    m_currentFilePath.isEmpty() ? "microcode.txt" : m_currentFilePath,
                                                    tr("Microcode Files (*.txt *.mc *.tsv)"));
    if (filePath.isEmpty())
        return;

    if (saveToFile(filePath)) {
        m_currentFilePath = filePath;
        setWindowTitle(QString("Microcode Editor - [%1]").arg(QFileInfo(filePath).fileName()));
    }
}

void MicrocodeEditorWindow::exitApp()
{
    if (maybeSave())
        close();
}

// ====================== SAVE / LOAD ======================
bool MicrocodeEditorWindow::maybeSave()
{
    return true; // simplify for now
}

bool MicrocodeEditorWindow::saveToFile(const QString& filePath)
{
    if (!m_editorWidget || !m_editorWidget->m_model)
        return false;

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, tr("Error"), tr("Cannot write file:\n%1").arg(filePath));
        return false;
    }

    QTextStream out(&file);
    out << "address\tlabel\talu\ts1\ts2\tdest\tconstant\tjcond\n";

    const Microcode* mc = m_editorWidget->m_model->microcode();
    if (!mc)
        return false;

    for (const auto& instr : mc->instructions) {
        out << instr.address  << '\t'
            << instr.label    << '\t'
            << instr.alu      << '\t'
            << instr.s1       << '\t'
            << instr.s2       << '\t'
            << instr.dest     << '\t'
            << instr.constant << '\t'
            << instr.jcond    << '\n';
    }

    file.close();
    return true;
}

bool MicrocodeEditorWindow::loadFromFile(const QString& filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, tr("Error"), tr("Cannot open file:\n%1").arg(filePath));
        return false;
    }

    QTextStream in(&file);
    QString headerLine = in.readLine(); // skip header
    if (headerLine.isEmpty())
        return false;

    Microcode* mc = new Microcode();
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty())
            continue;

        const QStringList parts = line.split('\t');
        if (parts.size() < 8)
            continue;

        Instruction instr;
        instr.address  = parts[0];
        instr.label    = parts[1];
        instr.alu      = parts[2];
        instr.s1       = parts[3];
        instr.s2       = parts[4];
        instr.dest     = parts[5];
        instr.constant = parts[6];
        instr.jcond    = parts[7];
        mc->instructions.append(instr);
    }

    file.close();
    m_editorWidget->m_model->setMicrocode(mc);
    return true;
}


