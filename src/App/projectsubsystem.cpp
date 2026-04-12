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
    QString projectFilename = QFileInfo(projectFile.filesystemFileName()).fileName();

    currentProject = projectFilename;

    qDebug() << "Current project: " << currentProject;

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

void ProjectSubsystem::newProject(){

    manifest = {0};
    currentProject = "";

    emit clearProject();
}

bool ProjectSubsystem::loadProject(QFile &projectFile)
{
    QStringList errors;
    parseProjectFile(projectFile, errors);

    emit loadConfigFile(manifest.instructionSetPath);
    emit loadMicrocodeFile(manifest.microcodePath);
    emit loadInstructionFile(manifest.instructionMemoryPath);
    emit loadMemoryFile(manifest.dataMemoryPath);

    emit projectLoaded();
    return true;
}

bool ProjectSubsystem::saveProject(QFile &projectFile)
{
    QStringList errors;
    parseProjectFile(projectFile, errors);

    emit saveConfigFile(manifest.instructionSetPath);
    emit saveMicrocodeFile(manifest.microcodePath);
    emit saveInstructionFile(manifest.instructionMemoryPath);
    emit saveMemoryFile(manifest.dataMemoryPath);

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


