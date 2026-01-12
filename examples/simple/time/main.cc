#include "examples/simple/time/time.h"

#include "xnet/base/Logging.h"
#include "xnet/net/EventLoop.h"

#include <unistd.h>

using namespace xnet;
using namespace xnet::net;

int main()
{
    LOG_INFO << "pid = " << getpid();
    EventLoop loop;
    InetAddress listenAddr(2037);
    TimeServer server(&loop, listenAddr);
    server.start();
    loop.loop();
}
