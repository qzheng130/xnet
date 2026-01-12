#ifndef XNET_EXAMPLES_HUB_PUBSUB_H
#define XNET_EXAMPLES_HUB_PUBSUB_H

#include "xnet/net/TcpClient.h"

namespace pubsub
{
using xnet::string;

// FIXME: dtor is not thread safe
class PubSubClient : xnet::noncopyable
{
public:
    typedef std::function<void(PubSubClient *)> ConnectionCallback;
    typedef std::function<void(const string &topic, const string &content, xnet::Timestamp)> SubscribeCallback;

    PubSubClient(xnet::net::EventLoop *loop, const xnet::net::InetAddress &hubAddr, const string &name);
    void start();
    void stop();
    bool connected() const;

    void setConnectionCallback(const ConnectionCallback &cb) { connectionCallback_ = cb; }

    bool subscribe(const string &topic, const SubscribeCallback &cb);
    void unsubscribe(const string &topic);
    bool publish(const string &topic, const string &content);

private:
    void onConnection(const xnet::net::TcpConnectionPtr &conn);
    void onMessage(const xnet::net::TcpConnectionPtr &conn, xnet::net::Buffer *buf, xnet::Timestamp receiveTime);
    bool send(const string &message);

    xnet::net::TcpClient client_;
    xnet::net::TcpConnectionPtr conn_;
    ConnectionCallback connectionCallback_;
    SubscribeCallback subscribeCallback_;
};
} // namespace pubsub

#endif // XNET_EXAMPLES_HUB_PUBSUB_H
