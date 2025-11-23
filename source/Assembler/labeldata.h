#ifndef LABELDATA_H
#define LABELDATA_H

namespace Assembly {

class LabelData{
private:
    QHash<QString, quint32> addressLookup;
    QHash<quint32, QString> labelLookup;

public:
    void setLabel(const QString &label, const quint32 address);
    bool removeLabel(const QString &label);
    bool removeLabel(const quint32 address);
    quint32 getAddress(const QString &label, bool *okptr) const;
    QString getLabel(const quint32 address, bool *okptr) const;
};

}
#endif // LABELDATA_H
