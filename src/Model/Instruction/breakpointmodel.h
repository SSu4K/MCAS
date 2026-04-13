#ifndef BREAKPOINTMODEL_H
#define BREAKPOINTMODEL_H

#include "breakpointdata.h"
#include "modellib_export.h"
#include "Common/texttablemodel.h"

namespace Models {

class MODELLIB_API BreakpointModel : public TextTableModel
{
    Q_OBJECT
public:
    explicit BreakpointModel(BreakpointEditor::BreakpointData* data,
                             QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent = {}) const override;
    int columnCount(const QModelIndex& parent = {}) const override;

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role) override;

    bool insertRows(int row, int count, const QModelIndex& parent = {}) override;
    bool removeRows(int row, int count, const QModelIndex& parent = {}) override;

    void clear();

protected:
    void populateFromStringMatrix(const QList<QList<QString>> &rows) override;
    bool isRowEmpty(const qsizetype row) const override;

private:
    BreakpointEditor::BreakpointData* m_data;
};

} // namespace Models

#endif // BREAKPOINTMODEL_H
