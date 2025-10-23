#include "texttablemodel.h"

QList<int> TextTableModel::computeColumnWidths() const
{
    QList<int> widths;
    widths.reserve(columnCount());

    for (int col = 0; col < columnCount(); col++) {
        int maxWidth = headerData(col, Qt::Horizontal, Qt::DisplayRole).toString().size();
        for (int row = 0; row < rowCount(); row++) {
            QString text = data(index(row, col), Qt::DisplayRole).toString();
            maxWidth = qMax(maxWidth, text.size());
        }
        widths.append(maxWidth);
    }
    return widths;
}

bool TextTableModel::saveToTextStream(QTextStream &stream) const
{
    if (!sectionHeader.isEmpty())
        stream << sectionHeader << "\n";

    auto columnWidths = computeColumnWidths();
    qsizetype width = 0;
    for (int col = 0; col < columnCount(); col++) {
        auto columnName = headerData(col, Qt::Horizontal, Qt::DisplayRole).toString();
        if(col == 0){
            columnName = ";" + columnName;
        }
        stream << columnName;

        if (col < columnCount() - 1){
            width = columnWidths[col] - columnName.size() + 2;
            stream << qSetFieldWidth(width) << delimiter << qSetFieldWidth(0);
        }
    }
    stream << "\n";
    for (int row = 0; row < rowCount(); row++) {
        for (int col = 0; col < columnCount(); col++) {
            QString cell = data(index(row, col), Qt::DisplayRole).toString();
            stream << cell;
            if (col < columnCount() - 1){
                width = columnWidths[col] - cell.size() + 2;
                stream << qSetFieldWidth(width) << delimiter << qSetFieldWidth(0);
            }
        }
        stream << "\n";
    }

    return true;
}

bool TextTableModel::findHeader(QTextStream &stream) const{
    if(sectionHeader.isEmpty()){
        return true;
    }
    while (!stream.atEnd()) {
        QString line = stream.readLine().trimmed();
        if (line == sectionHeader) {
            return true;
        }
    }
    return false;
}

bool TextTableModel::loadFromTextStream(QTextStream &stream)
{
    if(!findHeader(stream)){
        return false;
    }

    QList<QList<QString>> values;

    while (!stream.atEnd()) {
        QString line = stream.readLine().trimmed();
        if (line.isEmpty())
            continue;
        if (line.startsWith('['))
            break; // next section
        if (line.startsWith(';'))
            continue;

        QStringList parts = line.split(delimiter, Qt::KeepEmptyParts);
        for (QString &s : parts)
            s = s.trimmed();

        values.append(parts);
    }

    this->populateFromStringMatrix(values);
    return true;
}

void TextTableModel::populateFromStringMatrix(const QList<QList<QString>> &rows)
{
    Q_UNUSED(rows);
}
