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
public:
    explicit BreakpointData(int maxBreakpoints = 0);

    QList<BreakpointEntry> breakpoints;
    const int maxBreakpoints;

    void clear();
};

} // namespace BreakpointEditor
#endif // BREAKPOINTDATA_H
