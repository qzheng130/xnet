// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/xnet/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)
//
// This is a public header file, it must only include public header files.

#ifndef XNET_NET_HTTP_HTTPRESPONSE_H
#define XNET_NET_HTTP_HTTPRESPONSE_H

#include "xnet/base/copyable.h"
#include "xnet/base/Types.h"

#include <map>

namespace xnet
{
namespace net
{

class Buffer;

class HttpResponse : public xnet::copyable
{
public:
    enum HttpStatusCode
    {
        kUnknown,
        k200Ok = 200,
        k301MovedPermanently = 301,
        k400BadRequest = 400,
        k404NotFound = 404,
    };

    explicit HttpResponse(bool close)
    : statusCode_(kUnknown)
    , closeConnection_(close)
    {
    }

    void setStatusCode(HttpStatusCode code) { statusCode_ = code; }

    void setStatusMessage(const string &message) { statusMessage_ = message; }

    void setCloseConnection(bool on) { closeConnection_ = on; }

    bool closeConnection() const { return closeConnection_; }

    void setContentType(const string &contentType) { addHeader("Content-Type", contentType); }

    // FIXME: replace string with StringPiece
    void addHeader(const string &key, const string &value) { headers_[key] = value; }

    void setBody(const string &body) { body_ = body; }

    void appendToBuffer(Buffer *output) const;

private:
    std::map<string, string> headers_;
    HttpStatusCode statusCode_;
    // FIXME: add http version
    string statusMessage_;
    bool closeConnection_;
    string body_;
};

} // namespace net
} // namespace xnet

#endif // XNET_NET_HTTP_HTTPRESPONSE_H
