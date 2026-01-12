#include "xnet/net/EventLoop.h"

using namespace xnet;
using namespace xnet::net;

int main()
{
    EventLoop loop;
    loop.loop();
}
