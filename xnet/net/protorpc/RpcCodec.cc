// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/xnet/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)

#include "xnet/net/protorpc/RpcCodec.h"

#include "xnet/base/Logging.h"
#include "xnet/net/Endian.h"
#include "xnet/net/TcpConnection.h"

#include "xnet/net/protorpc/rpc.pb.h"
#include "xnet/net/protorpc/google-inl.h"

using namespace xnet;
using namespace xnet::net;

namespace
{
int ProtobufVersionCheck()
{
    GOOGLE_PROTOBUF_VERIFY_VERSION;
    return 0;
}

int dummy __attribute__((unused)) = ProtobufVersionCheck();
} // namespace

namespace xnet
{
namespace net
{
const char rpctag[] = "RPC0";
} // namespace net
} // namespace xnet
