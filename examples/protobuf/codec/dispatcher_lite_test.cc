#include "examples/protobuf/codec/dispatcher_lite.h"

#include "examples/protobuf/codec/query.pb.h"

#include <iostream>

using std::cout;
using std::endl;

void onUnknownMessageType(const xnet::net::TcpConnectionPtr &, const MessagePtr &message, xnet::Timestamp)
{
    cout << "onUnknownMessageType: " << message->GetTypeName() << endl;
}

void onQuery(const xnet::net::TcpConnectionPtr &, const MessagePtr &message, xnet::Timestamp)
{
    cout << "onQuery: " << message->GetTypeName() << endl;
    std::shared_ptr<xnet::Query> query = xnet::down_pointer_cast<xnet::Query>(message);
    assert(query != NULL);
}

void onAnswer(const xnet::net::TcpConnectionPtr &, const MessagePtr &message, xnet::Timestamp)
{
    cout << "onAnswer: " << message->GetTypeName() << endl;
    std::shared_ptr<xnet::Answer> answer = xnet::down_pointer_cast<xnet::Answer>(message);
    assert(answer != NULL);
}

int main()
{
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    ProtobufDispatcherLite dispatcher(onUnknownMessageType);
    dispatcher.registerMessageCallback(xnet::Query::descriptor(), onQuery);
    dispatcher.registerMessageCallback(xnet::Answer::descriptor(), onAnswer);

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
