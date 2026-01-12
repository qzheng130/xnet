#ifndef XNET_EXAMPLES_CDNS_RESOLVER_H
#define XNET_EXAMPLES_CDNS_RESOLVER_H

#include "xnet/base/noncopyable.h"
#include "xnet/base/StringPiece.h"
#include "xnet/base/Timestamp.h"
#include "xnet/net/InetAddress.h"

#include <functional>
#include <map>
#include <memory>

extern "C"
{
    struct hostent;
    struct ares_channeldata;
    typedef struct ares_channeldata *ares_channel;
}

namespace xnet
{
namespace net
{
class Channel;
class EventLoop;
} // namespace net
} // namespace xnet

namespace cdns
{

class Resolver : xnet::noncopyable
{
public:
    typedef std::function<void(const xnet::net::InetAddress &)> Callback;

    enum Option
    {
        kDNSandHostsFile,
        kDNSonly,
    };

    explicit Resolver(xnet::net::EventLoop *loop, Option opt = kDNSandHostsFile);
    ~Resolver();

    bool resolve(xnet::StringArg hostname, const Callback &cb);

private:
    struct QueryData
    {
        Resolver *owner;
        Callback callback;

        QueryData(Resolver *o, const Callback &cb)
        : owner(o)
        , callback(cb)
        {
        }
    };

    xnet::net::EventLoop *loop_;
    ares_channel ctx_;
    bool timerActive_;
    typedef std::map<int, std::unique_ptr<xnet::net::Channel>> ChannelList;
    ChannelList channels_;

    void onRead(int sockfd, xnet::Timestamp t);
    void onTimer();
    void onQueryResult(int status, struct hostent *result, const Callback &cb);
    void onSockCreate(int sockfd, int type);
    void onSockStateChange(int sockfd, bool read, bool write);

    static void ares_host_callback(void *data, int status, int timeouts, struct hostent *hostent);
    static int ares_sock_create_callback(int sockfd, int type, void *data);
    static void ares_sock_state_callback(void *data, int sockfd, int read, int write);
};
} // namespace cdns

#endif // XNET_EXAMPLES_CDNS_RESOLVER_H
