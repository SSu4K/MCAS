#include "breakpointdata.h"

namespace BreakpointEditor {

BreakpointData::BreakpointData(int maxBreakpoints)
    : maxBreakpoints(maxBreakpoints)
{
}

void BreakpointData::clear()
{
    if (breakpoints.isEmpty())
        return;

    breakpoints.clear();
}

} // namespace BreakpointEditor
