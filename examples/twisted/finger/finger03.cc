#include "xnet/net/EventLoop.h"
#include "xnet/net/TcpServer.h"

using namespace xnet;
using namespace xnet::net;

void onConnection(const TcpConnectionPtr &conn)
{
    if (conn->connected())
    {
        conn->shutdown();
    }
}

int main()
{
    EventLoop loop;
    TcpServer server(&loop, InetAddress(1079), "Finger");
    server.setConnectionCallback(onConnection);
    server.start();
    loop.loop();
}
