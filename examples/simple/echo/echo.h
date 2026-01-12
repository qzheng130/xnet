#ifndef XNET_EXAMPLES_SIMPLE_ECHO_ECHO_H
#define XNET_EXAMPLES_SIMPLE_ECHO_ECHO_H

#include "xnet/net/TcpServer.h"

// RFC 862
class EchoServer
{
public:
    EchoServer(xnet::net::EventLoop *loop, const xnet::net::InetAddress &listenAddr);

    void start(); // calls server_.start();

private:
    void onConnection(const xnet::net::TcpConnectionPtr &conn);

    void onMessage(const xnet::net::TcpConnectionPtr &conn, xnet::net::Buffer *buf, xnet::Timestamp time);

    xnet::net::TcpServer server_;
};

#endif // XNET_EXAMPLES_SIMPLE_ECHO_ECHO_H
