#include "effects.h"

namespace Sim{

    bool operator==(const RegWrite &l, const RegWrite &r){
        return (l.index == r.index) && (l.oldValue == r.oldValue) && (l.newValue == r.newValue);
    }
}
