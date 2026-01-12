// Copyright 2014, Shuo Chen.  All rights reserved.
// http://code.google.com/p/xnet/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)
//
// This is an internal header file, you should not include this.

#ifndef XNET_NET_INSPECT_SYSTEMINSPECTOR_H
#define XNET_NET_INSPECT_SYSTEMINSPECTOR_H

#include "xnet/net/inspect/Inspector.h"

namespace xnet
{
namespace net
{

class SystemInspector : noncopyable
{
public:
    void registerCommands(Inspector *ins);

    static string overview(HttpRequest::Method, const Inspector::ArgList &);
    static string loadavg(HttpRequest::Method, const Inspector::ArgList &);
    static string version(HttpRequest::Method, const Inspector::ArgList &);
    static string cpuinfo(HttpRequest::Method, const Inspector::ArgList &);
    static string meminfo(HttpRequest::Method, const Inspector::ArgList &);
    static string stat(HttpRequest::Method, const Inspector::ArgList &);
};

} // namespace net
} // namespace xnet

#endif // XNET_NET_INSPECT_SYSTEMINSPECTOR_H
