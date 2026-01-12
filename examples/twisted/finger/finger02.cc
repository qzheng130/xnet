#include "xnet/net/EventLoop.h"
#include "xnet/net/TcpServer.h"

using namespace xnet;
using namespace xnet::net;

int main()
{
    EventLoop loop;
    TcpServer server(&loop, InetAddress(1079), "Finger");
    server.start();
    loop.loop();
}
