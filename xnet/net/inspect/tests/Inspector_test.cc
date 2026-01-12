#include "xnet/net/inspect/Inspector.h"
#include "xnet/net/EventLoop.h"
#include "xnet/net/EventLoopThread.h"

using namespace xnet;
using namespace xnet::net;

int main()
{
    EventLoop loop;
    EventLoopThread t;
    Inspector ins(t.startLoop(), InetAddress(12345), "test");
    loop.loop();
}
