#include "examples/protobuf/codec/dispatcher.h"

#include "examples/protobuf/codec/query.pb.h"

#include <iostream>

using std::cout;
using std::endl;

typedef std::shared_ptr<xnet::Query> QueryPtr;
typedef std::shared_ptr<xnet::Answer> AnswerPtr;

void test_down_pointer_cast()
{
    ::std::shared_ptr<google::protobuf::Message> msg(new xnet::Query);
    ::std::shared_ptr<xnet::Query> query(xnet::down_pointer_cast<xnet::Query>(msg));
    assert(msg && query);
    if (!query)
    {
        abort();
    }
}

void onQuery(const xnet::net::TcpConnectionPtr &, const QueryPtr &message, xnet::Timestamp)
{
    cout << "onQuery: " << message->GetTypeName() << endl;
}

void onAnswer(const xnet::net::TcpConnectionPtr &, const AnswerPtr &message, xnet::Timestamp)
{
    cout << "onAnswer: " << message->GetTypeName() << endl;
}

void onUnknownMessageType(const xnet::net::TcpConnectionPtr &, const MessagePtr &message, xnet::Timestamp)
{
    cout << "onUnknownMessageType: " << message->GetTypeName() << endl;
}

int main()
{
    GOOGLE_PROTOBUF_VERIFY_VERSION;
    test_down_pointer_cast();

    ProtobufDispatcher dispatcher(onUnknownMessageType);
    dispatcher.registerMessageCallback<xnet::Query>(onQuery);
    dispatcher.registerMessageCallback<xnet::Answer>(onAnswer);

    xnet::net::TcpConnectionPtr conn;
    xnet::Timestamp t;

    std::shared_ptr<xnet::Query> query(new xnet::Query);
    std::shared_ptr<xnet::Answer> answer(new xnet::Answer);
    std::shared_ptr<xnet::Empty> empty(new xnet::Empty);
    dispatcher.onProtobufMessage(conn, query, t);
    dispatcher.onProtobufMessage(conn, answer, t);
    dispatcher.onProtobufMessage(conn, empty, t);

    google::protobuf::ShutdownProtobufLibrary();
}
