#ifndef PROJECTSUBSYSTEM_H
#define PROJECTSUBSYSTEM_H

#include "simulationsubsystem.h"
#include "modelssubsystem.h"

#include "Assembly/instructiondefinition.h"
#include "Microcode/microcodedata.h"
#include "Microcode/jumptabledata.h"
#include "Machine/machinestate.h"

struct ProjectManifest
{
    QString instructionSetPath;
    QString microcodePath;
    QString jumpTablePath;
    QString instructionMemoryPath;
    QString dataMemoryPath;

    // Future
    // QString machineConfigPath;
};

struct ProjectStagingContext
{
    // Domain-level temporary objects
    Assembly::InstructionSet instructionSet;
    Microcode::MicrocodeData microcode;
    Microcode::JumpTableData jumpTables;
    LabelData labelData;

    Machine::MachineState machineState;
    Machine::MachineConfig machineConfig;

    // Validation results
    QStringList errors;

    bool isValid() const { return errors.isEmpty(); }
};

class ProjectSubsystem : public QObject
{
    Q_OBJECT

public:
    explicit ProjectSubsystem(ModelsSubsystem& models,
                              QObject* parent = nullptr);

    bool hasOpenProject() const;
    QString currentProjectPath() const;

public slots:
    void newProject();
    bool loadProject(QFile &projectFile);
    bool saveProject(QFile &projectFile);

signals:
    void projectLoaded();
    void projectLoadFailed(const QStringList& errors);
    void projectSaved();

    void clearProject();

    bool saveMicrocodeFile(const QString &filePath);
    bool saveInstructionFile(const QString &filePath);
    bool saveMemoryFile(const QString &filePath);
    bool saveConfigFile(const QString &filePath);

    bool loadMicrocodeFile(const QString &filePath);
    bool loadInstructionFile(const QString &filePath);
    bool loadMemoryFile(const QString &filePath);
    bool loadConfigFile(const QString &filePath);

private:

    ModelsSubsystem& models;

    QString currentProject = "";
    ProjectManifest manifest;

    bool parseProjectFile(QFile &projectFile, QStringList& errors);
};


#endif // PROJECTSUBSYSTEM_H
