// Use of this source code is governed by a BSD-style license
// that can be found in the License file.
//
// Author: Shuo Chen (chenshuo at chenshuo dot com)

#ifndef XNET_BASE_ASYNCLOGGING_H
#define XNET_BASE_ASYNCLOGGING_H

#include "xnet/base/BlockingQueue.h"
#include "xnet/base/BoundedBlockingQueue.h"
#include "xnet/base/CountDownLatch.h"
#include "xnet/base/Mutex.h"
#include "xnet/base/Thread.h"
#include "xnet/base/LogStream.h"

#include <atomic>
#include <vector>

namespace xnet
{

class AsyncLogging : noncopyable
{
public:
    AsyncLogging(const string &basename, off_t rollSize, int flushInterval = 3);

    ~AsyncLogging()
    {
        if (running_)
        {
            stop();
        }
    }

    void append(const char *logline, int len);

    void start()
    {
        running_ = true;
        thread_.start();
        latch_.wait();
    }

    void stop() NO_THREAD_SAFETY_ANALYSIS
    {
        running_ = false;
        cond_.notify();
        thread_.join();
    }

private:
    void threadFunc();

    typedef xnet::detail::FixedBuffer<xnet::detail::kLargeBuffer> Buffer;
    typedef std::vector<std::unique_ptr<Buffer>> BufferVector;
    typedef BufferVector::value_type BufferPtr;

    const int flushInterval_;
    std::atomic<bool> running_;
    const string basename_;
    const off_t rollSize_;
    xnet::Thread thread_;
    xnet::CountDownLatch latch_;
    xnet::MutexLock mutex_;
    xnet::Condition cond_ GUARDED_BY(mutex_);
    BufferPtr currentBuffer_ GUARDED_BY(mutex_);
    BufferPtr nextBuffer_ GUARDED_BY(mutex_);
    BufferVector buffers_ GUARDED_BY(mutex_);
};

} // namespace xnet

#endif // XNET_BASE_ASYNCLOGGING_H
