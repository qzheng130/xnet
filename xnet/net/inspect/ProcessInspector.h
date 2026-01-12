// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/xnet/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)
//
// This is an internal header file, you should not include this.

#ifndef XNET_NET_INSPECT_PROCESSINSPECTOR_H
#define XNET_NET_INSPECT_PROCESSINSPECTOR_H

#include "xnet/net/inspect/Inspector.h"

namespace xnet
{
namespace net
{

class ProcessInspector : noncopyable
{
public:
    void registerCommands(Inspector *ins);

    static string overview(HttpRequest::Method, const Inspector::ArgList &);
    static string pid(HttpRequest::Method, const Inspector::ArgList &);
    static string procStatus(HttpRequest::Method, const Inspector::ArgList &);
    static string openedFiles(HttpRequest::Method, const Inspector::ArgList &);
    static string threads(HttpRequest::Method, const Inspector::ArgList &);

    static string username_;
};

} // namespace net
} // namespace xnet

#endif // XNET_NET_INSPECT_PROCESSINSPECTOR_H
