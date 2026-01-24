#ifndef TEXTTABLEMODEL_H
#define TEXTTABLEMODEL_H

#pragma once

#include <QAbstractTableModel>

class TextTableModel : public QAbstractTableModel
{
    Q_OBJECT

    public:
        explicit TextTableModel(QObject *parent = nullptr)
            : QAbstractTableModel(parent) {}

        virtual ~TextTableModel() = default;

        bool saveToTextStream(QTextStream &stream) const;
        bool loadFromTextStream(QTextStream &stream);

    protected:
        QString sectionHeader = QString();
        QChar headerPrefix = '[';
        QChar commentPrefix = ';';
        QChar delimiter = '|';

        virtual void populateFromStringMatrix(const QList<QList<QString>> &rows);
        virtual bool isRowEmpty(const qsizetype row) const;

        qsizetype computePopulatedRowCount() const;
        QList<int> computeColumnWidths() const;
        bool findHeader(QTextStream &stream) const;
};

#endif // TEXTTABLEMODEL_H
