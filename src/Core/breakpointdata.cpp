#include "breakpointdata.h"

namespace BreakpointEditor {

BreakpointData::BreakpointData(QObject *parent, int maxBreakpoints)
    : QObject(parent),
      maxBreakpoints(maxBreakpoints)
{
}

void BreakpointData::clear()
{
    if (breakpoints.isEmpty())
        return;

    breakpoints.clear();
    emit dataChanged();
}

} // namespace BreakpointEditor
