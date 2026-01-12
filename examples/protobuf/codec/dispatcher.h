// Copyright 2011, Shuo Chen.  All rights reserved.
// http://code.google.com/p/xnet/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.
//
// Author: Shuo Chen (chenshuo at chenshuo dot com)

#ifndef XNET_EXAMPLES_PROTOBUF_CODEC_DISPATCHER_H
#define XNET_EXAMPLES_PROTOBUF_CODEC_DISPATCHER_H

#include "xnet/base/noncopyable.h"
#include "xnet/net/Callbacks.h"

#include <google/protobuf/message.h>

#include <map>

#include <type_traits>

typedef std::shared_ptr<google::protobuf::Message> MessagePtr;

class Callback : xnet::noncopyable
{
public:
    virtual ~Callback() = default;
    virtual void onMessage(const xnet::net::TcpConnectionPtr &, const MessagePtr &message, xnet::Timestamp) const = 0;
};

template <typename T> class CallbackT : public Callback
{
    static_assert(std::is_base_of<google::protobuf::Message, T>::value, "T must be derived from gpb::Message.");

public:
    typedef std::function<void(const xnet::net::TcpConnectionPtr &, const std::shared_ptr<T> &message, xnet::Timestamp)>
        ProtobufMessageTCallback;

    CallbackT(const ProtobufMessageTCallback &callback)
    : callback_(callback)
    {
    }

    void onMessage(const xnet::net::TcpConnectionPtr &conn, const MessagePtr &message,
                   xnet::Timestamp receiveTime) const override
    {
        std::shared_ptr<T> concrete = xnet::down_pointer_cast<T>(message);
        assert(concrete != NULL);
        callback_(conn, concrete, receiveTime);
    }

private:
    ProtobufMessageTCallback callback_;
};

class ProtobufDispatcher
{
public:
    typedef std::function<void(const xnet::net::TcpConnectionPtr &, const MessagePtr &message, xnet::Timestamp)>
        ProtobufMessageCallback;

    explicit ProtobufDispatcher(const ProtobufMessageCallback &defaultCb)
    : defaultCallback_(defaultCb)
    {
    }

    void onProtobufMessage(const xnet::net::TcpConnectionPtr &conn, const MessagePtr &message,
                           xnet::Timestamp receiveTime) const
    {
        CallbackMap::const_iterator it = callbacks_.find(message->GetDescriptor());
        if (it != callbacks_.end())
        {
            it->second->onMessage(conn, message, receiveTime);
        }
        else
        {
            defaultCallback_(conn, message, receiveTime);
        }
    }

    template <typename T> void registerMessageCallback(const typename CallbackT<T>::ProtobufMessageTCallback &callback)
    {
        std::shared_ptr<CallbackT<T>> pd(new CallbackT<T>(callback));
        callbacks_[T::descriptor()] = pd;
    }

private:
    typedef std::map<const google::protobuf::Descriptor *, std::shared_ptr<Callback>> CallbackMap;

    CallbackMap callbacks_;
    ProtobufMessageCallback defaultCallback_;
};
#endif // XNET_EXAMPLES_PROTOBUF_CODEC_DISPATCHER_H
