// Use of this source code is governed by a BSD-style license
// that can be found in the License file.
//
// Author: Shuo Chen (chenshuo at chenshuo dot com)

#ifndef XNET_BASE_EXCEPTION_H
#define XNET_BASE_EXCEPTION_H

#include "xnet/base/Types.h"
#include <exception>

namespace xnet
{

class Exception : public std::exception
{
public:
    Exception(string what);
    ~Exception() noexcept override = default;

    // default copy-ctor and operator= are okay.

    const char *what() const noexcept override { return message_.c_str(); }

    const char *stackTrace() const noexcept { return stack_.c_str(); }

private:
    string message_;
    string stack_;
};

} // namespace xnet

#endif // XNET_BASE_EXCEPTION_H
