#ifndef XNET_EXAMPLES_SIMPLE_CHARGEN_CHARGEN_H
#define XNET_EXAMPLES_SIMPLE_CHARGEN_CHARGEN_H

#include "xnet/net/TcpServer.h"

// RFC 864
class ChargenServer
{
public:
    ChargenServer(xnet::net::EventLoop *loop, const xnet::net::InetAddress &listenAddr, bool print = false);

    void start();

private:
    void onConnection(const xnet::net::TcpConnectionPtr &conn);

    void onMessage(const xnet::net::TcpConnectionPtr &conn, xnet::net::Buffer *buf, xnet::Timestamp time);

    void onWriteComplete(const xnet::net::TcpConnectionPtr &conn);
    void printThroughput();

    xnet::net::TcpServer server_;

    xnet::string message_;
    int64_t transferred_;
    xnet::Timestamp startTime_;
};

#endif // XNET_EXAMPLES_SIMPLE_CHARGEN_CHARGEN_H
