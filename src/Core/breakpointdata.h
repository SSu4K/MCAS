#ifndef BREAKPOINTDATA_H
#define BREAKPOINTDATA_H

namespace BreakpointEditor {

enum class BreakpointType {
    PC,
    uAR
};

struct BreakpointEntry {
    BreakpointType type = BreakpointType::PC;
    quint32 value = 0;
    bool enabled = true;
};

class BreakpointData : public QObject
{
    Q_OBJECT
public:
    explicit BreakpointData(QObject *parent = nullptr, int maxBreakpoints = 0);

    QList<BreakpointEntry> breakpoints;
    const int maxBreakpoints;

    void clear();

signals:
    void dataChanged();
};

} // namespace BreakpointEditor
#endif // BREAKPOINTDATA_H
