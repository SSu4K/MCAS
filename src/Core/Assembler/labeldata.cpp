#include "labeldata.h"

using namespace Assembly;

void LabelData::setLabel(const QString &label, const quint32 address){
    addressLookup[label] = address;
    labelLookup[address] = label;
}

bool LabelData::removeLabel(const QString &label){
    if(!addressLookup.contains(label)){
        return false;
    }
    auto address = addressLookup[label];

    addressLookup.remove(label);
    labelLookup.remove(address);

    return true;
}

bool LabelData::removeLabel(const quint32 address){
    if(!labelLookup.contains(address)){
        return false;
    }
    auto label = labelLookup[address];

    addressLookup.remove(label);
    labelLookup.remove(address);

    return true;
}

quint32 LabelData::getAddress(const QString &label, bool *okptr) const {
    if(!addressLookup.contains(label)){
        *okptr = false;
        return 0;
    }

    *okptr = true;
    return addressLookup[label];
}

QString LabelData::getLabel(const quint32 address, bool *okptr) const {
    if(!labelLookup.contains(address)){
        *okptr = false;
        return 0;
    }

    *okptr = true;
    return labelLookup[address];
}
