#include "SysInfo.h"
#include <QtGlobal>

#ifdef Q_OS_WIN
#include "SysInfoWindowsImpl.h"
#endif

SysInfo& SysInfo::instance()
{

#ifdef Q_OS_WIN
    static SysInfoWindowsImpl s;
#endif

    return s;
}

SysInfo::SysInfo()
{

}

SysInfo::~SysInfo()
{

}
