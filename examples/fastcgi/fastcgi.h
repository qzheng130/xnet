#ifndef XNET_EXAMPLES_FASTCGI_FASTCGI_H
#define XNET_EXAMPLES_FASTCGI_FASTCGI_H

#include "xnet/net/TcpConnection.h"
#include <map>

// one FastCgiCodec per TcpConnection
// both lighttpd and nginx do not implement multiplexing,
// so there is no concurrent requests of one connection.
class FastCgiCodec : xnet::noncopyable
{
public:
    typedef std::map<std::string, std::string> ParamMap;
    typedef std::function<void(const xnet::net::TcpConnectionPtr &conn, ParamMap &, xnet::net::Buffer *)> Callback;

    explicit FastCgiCodec(const Callback &cb)
    : cb_(cb)
    , gotRequest_(false)
    , keepConn_(false)
    {
    }

    void onMessage(const xnet::net::TcpConnectionPtr &conn, xnet::net::Buffer *buf, xnet::Timestamp receiveTime)
    {
        parseRequest(buf);
        if (gotRequest_)
        {
            cb_(conn, params_, &stdin_);
            stdin_.retrieveAll();
            paramsStream_.retrieveAll();
            params_.clear();
            gotRequest_ = false;
            if (!keepConn_)
            {
                conn->shutdown();
            }
        }
    }

    static void respond(xnet::net::Buffer *response);

private:
    struct RecordHeader;
    bool parseRequest(xnet::net::Buffer *buf);
    bool onBeginRequest(const RecordHeader &header, const xnet::net::Buffer *buf);
    void onStdin(const char *content, uint16_t length);
    bool onParams(const char *content, uint16_t length);
    bool parseAllParams();
    uint32_t readLen();

    static void endStdout(xnet::net::Buffer *buf);
    static void endRequest(xnet::net::Buffer *buf);

    Callback cb_;
    bool gotRequest_;
    bool keepConn_;
    xnet::net::Buffer stdin_;
    xnet::net::Buffer paramsStream_;
    ParamMap params_;

    const static unsigned kRecordHeader;
};

#endif // XNET_EXAMPLES_FASTCGI_FASTCGI_H
