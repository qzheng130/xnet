// Use of this source code is governed by a BSD-style license
// that can be found in the License file.
//
// Author: Shuo Chen (chenshuo at chenshuo dot com)

#include "xnet/base/Exception.h"
#include "xnet/base/CurrentThread.h"

namespace xnet
{

Exception::Exception(string msg)
: message_(std::move(msg))
, stack_(CurrentThread::stackTrace(/*demangle=*/false))
{
}

} // namespace xnet
