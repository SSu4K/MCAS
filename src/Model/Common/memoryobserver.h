#ifndef MEMORYOBSERVER_H
#define MEMORYOBSERVER_H

// Class for models to derive from so that they can laterally signal memory changes to eachother.
// Due to Qt limitations each child has to define the signal

class MemoryObserver
{
    // signals:
    //     void memoryRegionChanged(quint32 startAddress, quint32 endAddress);

// public:

//     virtual void onMemoryRegionChanged(const quint32 startAddress, const quint32 endAddress) = 0;
};

#endif // MEMORYOBSERVER_H
