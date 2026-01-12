#include "xnet/base/Logging.h"
#include "xnet/net/EventLoop.h"
#include "xnet/net/TcpServer.h"
#include "examples/asio/chat/codec.h"

#include <stdio.h>
#include <unistd.h>

using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;
using xnet::get_pointer;

bool g_tcpNoDelay = false;

void onConnection(const xnet::net::TcpConnectionPtr &conn)
{
    if (conn->connected())
    {
        conn->setTcpNoDelay(g_tcpNoDelay);
    }
}

void onStringMessage(LengthHeaderCodec *codec, const xnet::net::TcpConnectionPtr &conn, const xnet::string &message,
                     xnet::Timestamp)
{
    codec->send(get_pointer(conn), message);
}

int main(int argc, char *argv[])
{
    if (argc > 1)
    {
        uint16_t port = static_cast<uint16_t>(atoi(argv[1]));
        g_tcpNoDelay = argc > 2 ? atoi(argv[2]) : false;
        int threadCount = argc > 3 ? atoi(argv[3]) : 0;

        LOG_INFO << "pid = " << getpid() << ", listen port = " << port;
        // xnet::Logger::setLogLevel(xnet::Logger::WARN);
        xnet::net::EventLoop loop;
        xnet::net::InetAddress listenAddr(port);
        xnet::net::TcpServer server(&loop, listenAddr, "PingPong");
        LengthHeaderCodec codec(std::bind(onStringMessage, &codec, _1, _2, _3));

        server.setConnectionCallback(onConnection);
        server.setMessageCallback(std::bind(&LengthHeaderCodec::onMessage, &codec, _1, _2, _3));

        if (threadCount > 1)
        {
            server.setThreadNum(threadCount);
        }

        server.start();

        loop.loop();
    }
    else
    {
        fprintf(stderr, "Usage: %s listen_port [tcp_no_delay [threads]]\n", argv[0]);
    }
}
