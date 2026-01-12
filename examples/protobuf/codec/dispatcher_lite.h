// Copyright 2011, Shuo Chen.  All rights reserved.
// http://code.google.com/p/xnet/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.
//
// Author: Shuo Chen (chenshuo at chenshuo dot com)

#ifndef XNET_EXAMPLES_PROTOBUF_CODEC_DISPATCHER_LITE_H
#define XNET_EXAMPLES_PROTOBUF_CODEC_DISPATCHER_LITE_H

#include "xnet/base/noncopyable.h"
#include "xnet/net/Callbacks.h"

#include <google/protobuf/message.h>

#include <map>

typedef std::shared_ptr<google::protobuf::Message> MessagePtr;

class ProtobufDispatcherLite : xnet::noncopyable
{
public:
    typedef std::function<void(const xnet::net::TcpConnectionPtr &, const MessagePtr &, xnet::Timestamp)>
        ProtobufMessageCallback;

    // ProtobufDispatcher()
    //   : defaultCallback_(discardProtobufMessage)
    // {
    // }

    explicit ProtobufDispatcherLite(const ProtobufMessageCallback &defaultCb)
    : defaultCallback_(defaultCb)
    {
    }

    void onProtobufMessage(const xnet::net::TcpConnectionPtr &conn, const MessagePtr &message,
                           xnet::Timestamp receiveTime) const
    {
        CallbackMap::const_iterator it = callbacks_.find(message->GetDescriptor());
        if (it != callbacks_.end())
        {
            it->second(conn, message, receiveTime);
        }
        else
        {
            defaultCallback_(conn, message, receiveTime);
        }
    }

    void registerMessageCallback(const google::protobuf::Descriptor *desc, const ProtobufMessageCallback &callback)
    {
        callbacks_[desc] = callback;
    }

private:
    // static void discardProtobufMessage(const xnet::net::TcpConnectionPtr&,
    //                                    const MessagePtr&,
    //                                    xnet::Timestamp);

    typedef std::map<const google::protobuf::Descriptor *, ProtobufMessageCallback> CallbackMap;
    CallbackMap callbacks_;
    ProtobufMessageCallback defaultCallback_;
};

#endif // XNET_EXAMPLES_PROTOBUF_CODEC_DISPATCHER_LITE_H
