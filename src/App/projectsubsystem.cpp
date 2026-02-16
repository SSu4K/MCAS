#include "projectsubsystem.h"

ProjectSubsystem::ProjectSubsystem(ModelsSubsystem& mod,
                                   QObject* parent)
    : QObject(parent), models(mod), manifest()
{
    qDebug() << "Init project subsystem!";
}

bool ProjectSubsystem::parseProjectFile(QFile &projectFile, QStringList& errors)
{
    QTextStream in(&projectFile);
    QDir baseDir = QFileInfo(projectFile.filesystemFileName()).dir();

    while (!in.atEnd()) {
        const QString line = in.readLine().trimmed();
        if (line.isEmpty() || line.startsWith('#'))
            continue;

        const auto parts = line.split('=');
        if (parts.size() != 2) {
            errors << "Invalid line in project file: " + line;
            return false;
        }

        const QString key = parts[0].trimmed();
        const QString value = baseDir.filePath(parts[1].trimmed());

        if (key == "InstructionSet")       this->manifest.instructionSetPath = value;
        else if (key == "Microcode")        this->manifest.microcodePath = value;
        else if (key == "JumpTable")        this->manifest.jumpTablePath = value;
        else if (key == "InstructionMemory") this->manifest.instructionMemoryPath = value;
        else if (key == "DataMemory")       this->manifest.dataMemoryPath = value;
        else {
            errors << "Unknown project key: " + key;
            return false;
        }
    }

    return true;
}

bool ProjectSubsystem::loadProject(QFile &projectFile)
{
    QStringList errors;
    parseProjectFile(projectFile, errors);

    QFile instrFile(manifest.instructionSetPath);
    if (instrFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream s(&instrFile);
        models.instructionSetModel.loadFromTextStream(s); // assumes your method exists
    }

    QFile microFile(manifest.microcodePath);
    if (microFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream s(&microFile);
        models.microcodeModel.loadFromTextStream(s);
    }

    QFile jumpFile(manifest.jumpTablePath);
    if (jumpFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream s(&jumpFile);
        models.jumpTableModel.loadFromTextStream(s);
    }

    QFile instrMemFile(manifest.instructionMemoryPath);
    if (instrMemFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream s(&instrMemFile);
        models.instructionModel.loadFromTextStream(s);
    }

    QFile dataMemFile(manifest.dataMemoryPath);
    if (dataMemFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream s(&dataMemFile);
        models.memoryModel.loadFromTextStream(s);
    }

    emit projectLoaded();
    return true;
}

bool ProjectSubsystem::saveProject(const QString& projectFilePath)
{
    Q_UNUSED(projectFilePath);
    emit projectSaved();
    return true;
}

bool ProjectSubsystem::hasOpenProject() const
{
    return !currentProject.isEmpty();
}

QString ProjectSubsystem::currentProjectPath() const
{
    return currentProject;
}


