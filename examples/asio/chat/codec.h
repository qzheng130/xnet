#ifndef XNET_EXAMPLES_ASIO_CHAT_CODEC_H
#define XNET_EXAMPLES_ASIO_CHAT_CODEC_H

#include "xnet/base/Logging.h"
#include "xnet/net/Buffer.h"
#include "xnet/net/Endian.h"
#include "xnet/net/TcpConnection.h"

class LengthHeaderCodec : xnet::noncopyable
{
public:
    typedef std::function<void(const xnet::net::TcpConnectionPtr &, const xnet::string &message, xnet::Timestamp)>
        StringMessageCallback;

    explicit LengthHeaderCodec(const StringMessageCallback &cb)
    : messageCallback_(cb)
    {
    }

    void onMessage(const xnet::net::TcpConnectionPtr &conn, xnet::net::Buffer *buf, xnet::Timestamp receiveTime)
    {
        while (buf->readableBytes() >= kHeaderLen) // kHeaderLen == 4
        {
            // FIXME: use Buffer::peekInt32()
            const void *data = buf->peek();
            int32_t be32 = *static_cast<const int32_t *>(data); // SIGBUS
            const int32_t len = xnet::net::sockets::networkToHost32(be32);
            if (len > 65536 || len < 0)
            {
                LOG_ERROR << "Invalid length " << len;
                conn->shutdown(); // FIXME: disable reading
                break;
            }
            else if (buf->readableBytes() >= len + kHeaderLen)
            {
                buf->retrieve(kHeaderLen);
                xnet::string message(buf->peek(), len);
                messageCallback_(conn, message, receiveTime);
                buf->retrieve(len);
            }
            else
            {
                break;
            }
        }
    }

    // FIXME: TcpConnectionPtr
    void send(xnet::net::TcpConnection *conn, const xnet::StringPiece &message)
    {
        xnet::net::Buffer buf;
        buf.append(message.data(), message.size());
        int32_t len = static_cast<int32_t>(message.size());
        int32_t be32 = xnet::net::sockets::hostToNetwork32(len);
        buf.prepend(&be32, sizeof be32);
        conn->send(&buf);
    }

private:
    StringMessageCallback messageCallback_;
    const static size_t kHeaderLen = sizeof(int32_t);
};

#endif // XNET_EXAMPLES_ASIO_CHAT_CODEC_H
