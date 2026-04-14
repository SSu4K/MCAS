#include "projectsubsystem.h"

static const QString INSTRUCTION_SET_KEY    = "InstructionSet";
static const QString MICROCODE_KEY          = "Microcode";
static const QString MEMORY_KEY             = "DataMemory";
static const QString INSTRUCTIONS_KEY       = "InstructionMemory";

ProjectSubsystem::ProjectSubsystem(ModelsSubsystem& mod,
                                   QObject* parent)
    : QObject(parent), models(mod), manifest(){}

bool ProjectSubsystem::parseProjectFile(QFile &projectFile, QStringList& errors)
{
    QTextStream in(&projectFile);
    QDir baseDir = QFileInfo(projectFile.filesystemFileName()).dir();
    QString projectFilename = QFileInfo(projectFile.filesystemFileName()).fileName();


    newProject();
    currentProject = projectFilename;

    while (!in.atEnd()) {
        const QString line = in.readLine().trimmed();
        if (line.isEmpty() || line.startsWith('#'))
            continue;

        const auto parts = line.split('=');
        if (parts.size() != 2) {
            errors << "Invalid line in project file: " + line;
            return false;
        }

        const QString filename = parts[1].trimmed();
        if(filename.isEmpty()) continue;

        const QString key = parts[0].trimmed();
        const QString value = baseDir.filePath(filename);

        if (key == INSTRUCTION_SET_KEY)     this->manifest.instructionSetPath = value;
        else if (key == MICROCODE_KEY)      this->manifest.microcodePath = value;
        else if (key == MEMORY_KEY)         this->manifest.dataMemoryPath = value;
        else if (key == INSTRUCTIONS_KEY)   this->manifest.instructionMemoryPath = value;
        else {
            errors << "Unknown project key: " + key;
            return false;
        }
    }

    return true;
}

bool ProjectSubsystem::writeProjectFile(QFile &projectFile){
    QTextStream out(&projectFile);

    out << INSTRUCTION_SET_KEY  << "=" << manifest.instructionSetPath.split('/').last() << Qt::endl;
    out << MICROCODE_KEY        << "=" << manifest.microcodePath.split('/').last() << Qt::endl;
    out << MEMORY_KEY           << "=" << manifest.dataMemoryPath.split('/').last() << Qt::endl;
    out << INSTRUCTIONS_KEY     << "=" << manifest.instructionMemoryPath.split('/').last() << Qt::endl;

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
    emit loadMemoryFile(manifest.dataMemoryPath);
    emit loadInstructionFile(manifest.instructionMemoryPath);

    emit projectLoaded();
    return true;
}

bool ProjectSubsystem::saveProject(QFile &projectFile)
{
    QStringList errors;

    emit saveConfigFile(manifest.instructionSetPath);
    emit saveMicrocodeFile(manifest.microcodePath);
    emit saveMemoryFile(manifest.dataMemoryPath);
    emit saveInstructionFile(manifest.instructionMemoryPath);

    if(projectFile.open(QIODevice::WriteOnly | QIODevice::Text)){
        writeProjectFile(projectFile);
        projectFile.close();

        emit projectSaved();
        return true;
    }

    qDebug() << "Project file not found!" << projectFile.fileName();
    return true;
}

void ProjectSubsystem::microcodeFileChanged(const QString &filePath){
    manifest.microcodePath = filePath;
    qDebug() << "manifest changed microcode:" << filePath;
}

void ProjectSubsystem::instructionFileChanged(const QString &filePath){
    manifest.instructionMemoryPath = filePath;
    qDebug() << "manifest changed instruction:" << filePath;
}

void ProjectSubsystem::memoryFileChanged(const QString &filePath){
    manifest.dataMemoryPath = filePath;
    qDebug() << "manifest changed memory:" << filePath;
}

void ProjectSubsystem::configFileChanged(const QString &filePath){
    manifest.instructionSetPath = filePath;
    qDebug() << "manifest changed config:" << filePath;
}

bool ProjectSubsystem::hasOpenProject() const
{
    return !currentProject.isEmpty();
}

QString ProjectSubsystem::currentProjectPath() const
{
    return currentProject;
}


