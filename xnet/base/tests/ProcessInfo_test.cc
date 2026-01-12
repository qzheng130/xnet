#include "xnet/base/ProcessInfo.h"
#include <stdio.h>
#define __STDC_FORMAT_MACROS
#include <inttypes.h>

int main()
{
    printf("pid = %d\n", xnet::ProcessInfo::pid());
    printf("uid = %d\n", xnet::ProcessInfo::uid());
    printf("euid = %d\n", xnet::ProcessInfo::euid());
    printf("start time = %s\n", xnet::ProcessInfo::startTime().toFormattedString().c_str());
    printf("hostname = %s\n", xnet::ProcessInfo::hostname().c_str());
    printf("opened files = %d\n", xnet::ProcessInfo::openedFiles());
    printf("threads = %zd\n", xnet::ProcessInfo::threads().size());
    printf("num threads = %d\n", xnet::ProcessInfo::numThreads());
    printf("status = %s\n", xnet::ProcessInfo::procStatus().c_str());
}
