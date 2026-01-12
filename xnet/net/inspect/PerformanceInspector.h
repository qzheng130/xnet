// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/xnet/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)
//
// This is an internal header file, you should not include this.

#ifndef XNET_NET_INSPECT_PERFORMANCEINSPECTOR_H
#define XNET_NET_INSPECT_PERFORMANCEINSPECTOR_H

#include "xnet/net/inspect/Inspector.h"

namespace xnet
{
namespace net
{

class PerformanceInspector : noncopyable
{
public:
    void registerCommands(Inspector *ins);

    static string heap(HttpRequest::Method, const Inspector::ArgList &);
    static string growth(HttpRequest::Method, const Inspector::ArgList &);
    static string profile(HttpRequest::Method, const Inspector::ArgList &);
    static string cmdline(HttpRequest::Method, const Inspector::ArgList &);
    static string memstats(HttpRequest::Method, const Inspector::ArgList &);
    static string memhistogram(HttpRequest::Method, const Inspector::ArgList &);
    static string releaseFreeMemory(HttpRequest::Method, const Inspector::ArgList &);

    static string symbol(HttpRequest::Method, const Inspector::ArgList &);
};

} // namespace net
} // namespace xnet

#endif // XNET_NET_INSPECT_PERFORMANCEINSPECTOR_H
