#ifndef SYSINFOWINDOWSIMPL_H
#define SYSINFOWINDOWSIMPL_H

#include <QtGlobal>
#include <QVector>
#include "SysInfo.h"

typedef struct _FILETIME FILETIME;


class SysInfoWindowsImpl : public SysInfo
{
public:
    SysInfoWindowsImpl();

    // SysInfo interface
public:
    void init() override;
    double cpuLoadAverage() override;
    double memoryUsed() override;

private:
    QVector<qulonglong> cpuRawData();
    qlonglong convertFileTime(const FILETIME &filetime) const;

private:
    QVector<qulonglong> mCpuLoadLastValues;
};

#endif // SYSINFOWINDOWSIMPL_H
