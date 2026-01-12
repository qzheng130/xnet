#include "examples/simple/discard/discard.h"

#include "xnet/base/Logging.h"
#include "xnet/net/EventLoop.h"

#include <unistd.h>

using namespace xnet;
using namespace xnet::net;

int main()
{
    LOG_INFO << "pid = " << getpid();
    EventLoop loop;
    InetAddress listenAddr(2009);
    DiscardServer server(&loop, listenAddr);
    server.start();
    loop.loop();
}
