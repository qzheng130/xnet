#include "examples/protobuf/codec/dispatcher.h"
#include "examples/protobuf/codec/codec.h"
#include "examples/protobuf/codec/query.pb.h"

#include "xnet/base/Logging.h"
#include "xnet/base/Mutex.h"
#include "xnet/net/EventLoop.h"
#include "xnet/net/TcpClient.h"

#include <stdio.h>
#include <unistd.h>

using namespace xnet;
using namespace xnet::net;

typedef std::shared_ptr<xnet::Empty> EmptyPtr;
typedef std::shared_ptr<xnet::Answer> AnswerPtr;

google::protobuf::Message *messageToSend;

class QueryClient : noncopyable
{
public:
    QueryClient(EventLoop *loop, const InetAddress &serverAddr)
    : loop_(loop)
    , client_(loop, serverAddr, "QueryClient")
    , dispatcher_(std::bind(&QueryClient::onUnknownMessage, this, _1, _2, _3))
    , codec_(std::bind(&ProtobufDispatcher::onProtobufMessage, &dispatcher_, _1, _2, _3))
    {
        dispatcher_.registerMessageCallback<xnet::Answer>(std::bind(&QueryClient::onAnswer, this, _1, _2, _3));
        dispatcher_.registerMessageCallback<xnet::Empty>(std::bind(&QueryClient::onEmpty, this, _1, _2, _3));
        client_.setConnectionCallback(std::bind(&QueryClient::onConnection, this, _1));
        client_.setMessageCallback(std::bind(&ProtobufCodec::onMessage, &codec_, _1, _2, _3));
    }

    void connect() { client_.connect(); }

private:
    void onConnection(const TcpConnectionPtr &conn)
    {
        LOG_INFO << conn->localAddress().toIpPort() << " -> " << conn->peerAddress().toIpPort() << " is "
                 << (conn->connected() ? "UP" : "DOWN");

        if (conn->connected())
        {
            codec_.send(conn, *messageToSend);
        }
        else
        {
            loop_->quit();
        }
    }

    void onUnknownMessage(const TcpConnectionPtr &, const MessagePtr &message, Timestamp)
    {
        LOG_INFO << "onUnknownMessage: " << message->GetTypeName();
    }

    void onAnswer(const xnet::net::TcpConnectionPtr &, const AnswerPtr &message, xnet::Timestamp)
    {
        LOG_INFO << "onAnswer:\n" << message->GetTypeName() << message->DebugString();
    }

    void onEmpty(const xnet::net::TcpConnectionPtr &, const EmptyPtr &message, xnet::Timestamp)
    {
        LOG_INFO << "onEmpty: " << message->GetTypeName();
    }

    EventLoop *loop_;
    TcpClient client_;
    ProtobufDispatcher dispatcher_;
    ProtobufCodec codec_;
};

int main(int argc, char *argv[])
{
    LOG_INFO << "pid = " << getpid();
    if (argc > 2)
    {
        EventLoop loop;
        uint16_t port = static_cast<uint16_t>(atoi(argv[2]));
        InetAddress serverAddr(argv[1], port);

        xnet::Query query;
        query.set_id(1);
        query.set_questioner("Chen Shuo");
        query.add_question("Running?");
        xnet::Empty empty;
        messageToSend = &query;

        if (argc > 3 && argv[3][0] == 'e')
        {
            messageToSend = &empty;
        }

        QueryClient client(&loop, serverAddr);
        client.connect();
        loop.loop();
    }
    else
    {
        printf("Usage: %s host_ip port [q|e]\n", argv[0]);
    }
}
