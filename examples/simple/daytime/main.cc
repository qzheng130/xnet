#include "examples/simple/daytime/daytime.h"

#include "xnet/base/Logging.h"
#include "xnet/net/EventLoop.h"

#include <unistd.h>

using namespace xnet;
using namespace xnet::net;

int main()
{
    LOG_INFO << "pid = " << getpid();
    EventLoop loop;
    InetAddress listenAddr(2013);
    DaytimeServer server(&loop, listenAddr);
    server.start();
    loop.loop();
}
