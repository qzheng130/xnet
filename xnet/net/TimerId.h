// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/xnet/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)
//
// This is a public header file, it must only include public header files.

#ifndef XNET_NET_TIMERID_H
#define XNET_NET_TIMERID_H

#include "xnet/base/copyable.h"

namespace xnet
{
namespace net
{

class Timer;

///
/// An opaque identifier, for canceling Timer.
///
class TimerId : public xnet::copyable
{
public:
    TimerId()
    : timer_(NULL)
    , sequence_(0)
    {
    }

    TimerId(Timer *timer, int64_t seq)
    : timer_(timer)
    , sequence_(seq)
    {
    }

    // default copy-ctor, dtor and assignment are okay

    friend class TimerQueue;

private:
    Timer *timer_;
    int64_t sequence_;
};

} // namespace net
} // namespace xnet

#endif // XNET_NET_TIMERID_H
