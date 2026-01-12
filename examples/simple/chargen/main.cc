#include "examples/simple/chargen/chargen.h"

#include "xnet/base/Logging.h"
#include "xnet/net/EventLoop.h"

#include <unistd.h>

using namespace xnet;
using namespace xnet::net;

int main()
{
    LOG_INFO << "pid = " << getpid();
    EventLoop loop;
    InetAddress listenAddr(2019);
    ChargenServer server(&loop, listenAddr, true);
    server.start();
    loop.loop();
}
