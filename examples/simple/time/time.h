#ifndef XNET_EXAMPLES_SIMPLE_TIME_TIME_H
#define XNET_EXAMPLES_SIMPLE_TIME_TIME_H

#include "xnet/net/TcpServer.h"

// RFC 868
class TimeServer
{
public:
    TimeServer(xnet::net::EventLoop *loop, const xnet::net::InetAddress &listenAddr);

    void start();

private:
    void onConnection(const xnet::net::TcpConnectionPtr &conn);

    void onMessage(const xnet::net::TcpConnectionPtr &conn, xnet::net::Buffer *buf, xnet::Timestamp time);

    xnet::net::TcpServer server_;
};

#endif // XNET_EXAMPLES_SIMPLE_TIME_TIME_H
