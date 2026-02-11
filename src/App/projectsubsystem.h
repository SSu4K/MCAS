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
    explicit ProjectSubsystem(SimulationSubsystem& simulation,
                              ModelsSubsystem& models,
                              QObject* parent = nullptr);

    // --- Project lifecycle ---
    bool loadProject(const QString& projectFilePath);
    bool saveProject(const QString& projectFilePath);

    bool hasOpenProject() const;
    QString currentProjectPath() const;

signals:
    void projectLoaded();
    void projectLoadFailed(const QStringList& errors);
    void projectSaved();

private:
    // --- Core references ---
    SimulationSubsystem& simulation;
    ModelsSubsystem& models;

    // --- State ---
    QString currentProject;
    ProjectManifest manifest;

    // --- Load pipeline ---
    bool parseProjectFile(const QString& path, ProjectManifest& outManifest, QStringList& errors);
    bool stageProject(const ProjectManifest& manifest, ProjectStagingContext& staging);
    bool validateStaging(ProjectStagingContext& staging);

    void commitStaging(ProjectStagingContext&& staging);
    void rollbackStaging(); // conceptual, usually empty

    // --- Save helpers ---
    bool writeProjectFile(const QString& path, QStringList& errors);
};


#endif // PROJECTSUBSYSTEM_H
