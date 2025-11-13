#ifndef INSTRUCTIONDATA_H
#define INSTRUCTIONDATA_H

#include <QObject>
#include "instruction.h"

namespace InstructionEditor {

const qsizetype MAX_LINES = 512;
const quint32 BASE_ADDRESS = 0;

struct InstructionEntry {
    QByteArray encoded = 0;
    QString text = "NOP";
    bool valid = true;
    QString errorMessage;
};

class InstructionData : public QObject
{
    Q_OBJECT
public:
    QList<InstructionEntry> instructions;
    const int maxLines;
    quint32 baseAddress;
    explicit InstructionData(QObject *parent = nullptr, int maxLines = MAX_LINES, quint32 baseAddress = BASE_ADDRESS);

signals:
};

} // namespace InstructionEditor

#endif // INSTRUCTIONDATA_H
