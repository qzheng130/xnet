#include "examples/simple/echo/echo.h"

#include "xnet/base/Logging.h"
#include "xnet/net/EventLoop.h"

#include <unistd.h>

// using namespace xnet;
// using namespace xnet::net;

int main()
{
    LOG_INFO << "pid = " << getpid();
    xnet::net::EventLoop loop;
    xnet::net::InetAddress listenAddr(2007);
    EchoServer server(&loop, listenAddr);
    server.start();
    loop.loop();
}
