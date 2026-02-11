#include "projectsubsystem.h"

bool ProjectSubsystem::parseProjectFile(const QString& path,
                                        ProjectManifest& outManifest,
                                        QStringList& errors)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        errors << "Failed to open project file";
        return false;
    }

    QTextStream in(&file);
    QDir baseDir = QFileInfo(path).dir();

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

        if (key == "InstructionSet")       outManifest.instructionSetPath = value;
        else if (key == "Microcode")        outManifest.microcodePath = value;
        else if (key == "JumpTable")        outManifest.jumpTablePath = value;
        else if (key == "InstructionMemory") outManifest.instructionMemoryPath = value;
        else if (key == "DataMemory")       outManifest.dataMemoryPath = value;
        else {
            errors << "Unknown project key: " + key;
            return false;
        }
    }

    return true;
}

