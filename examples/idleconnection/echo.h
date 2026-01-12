#ifndef XNET_EXAMPLES_IDLECONNECTION_ECHO_H
#define XNET_EXAMPLES_IDLECONNECTION_ECHO_H

#include "xnet/net/TcpServer.h"
// #include <xnet/base/Types.h>

#include <unordered_set>

#include <boost/circular_buffer.hpp>

// RFC 862
class EchoServer
{
public:
    EchoServer(xnet::net::EventLoop *loop, const xnet::net::InetAddress &listenAddr, int idleSeconds);

    void start();

private:
    void onConnection(const xnet::net::TcpConnectionPtr &conn);

    void onMessage(const xnet::net::TcpConnectionPtr &conn, xnet::net::Buffer *buf, xnet::Timestamp time);

    void onTimer();

    void dumpConnectionBuckets() const;

    typedef std::weak_ptr<xnet::net::TcpConnection> WeakTcpConnectionPtr;

    struct Entry : public xnet::copyable
    {
        explicit Entry(const WeakTcpConnectionPtr &weakConn)
        : weakConn_(weakConn)
        {
        }

        ~Entry()
        {
            xnet::net::TcpConnectionPtr conn = weakConn_.lock();
            if (conn)
            {
                conn->shutdown();
            }
        }

        WeakTcpConnectionPtr weakConn_;
    };

    typedef std::shared_ptr<Entry> EntryPtr;
    typedef std::weak_ptr<Entry> WeakEntryPtr;
    typedef std::unordered_set<EntryPtr> Bucket;
    typedef boost::circular_buffer<Bucket> WeakConnectionList;

    xnet::net::TcpServer server_;
    WeakConnectionList connectionBuckets_;
};

#endif // XNET_EXAMPLES_IDLECONNECTION_ECHO_H
