#include "xnet/net/EventLoop.h"

#include <iostream>

void print() { std::cout << "Hello, world!\n"; }

int main()
{
    xnet::net::EventLoop loop;
    loop.runAfter(5, print);
    loop.loop();
}
