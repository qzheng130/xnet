#ifndef XNET_EXAMPLES_HUB_CODEC_H
#define XNET_EXAMPLES_HUB_CODEC_H

// internal header file

#include "xnet/base/Types.h"
#include "xnet/net/Buffer.h"

namespace pubsub
{
using xnet::string;

enum ParseResult
{
    kError,
    kSuccess,
    kContinue,
};

ParseResult parseMessage(xnet::net::Buffer *buf, string *cmd, string *topic, string *content);
} // namespace pubsub

#endif // XNET_EXAMPLES_HUB_CODEC_H
