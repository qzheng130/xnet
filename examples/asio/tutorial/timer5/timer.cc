#include "xnet/base/Mutex.h"
#include "xnet/net/EventLoop.h"
#include "xnet/net/EventLoopThread.h"

#include <iostream>

class Printer : xnet::noncopyable
{
public:
    Printer(xnet::net::EventLoop *loop1, xnet::net::EventLoop *loop2)
    : loop1_(loop1)
    , loop2_(loop2)
    , count_(0)
    {
        loop1_->runAfter(1, std::bind(&Printer::print1, this));
        loop2_->runAfter(1, std::bind(&Printer::print2, this));
    }

    ~Printer() { std::cout << "Final count is " << count_ << "\n"; }

    void print1()
    {
        xnet::MutexLockGuard lock(mutex_);
        if (count_ < 10)
        {
            std::cout << "Timer 1: " << count_ << "\n";
            ++count_;

            loop1_->runAfter(1, std::bind(&Printer::print1, this));
        }
        else
        {
            loop1_->quit();
        }
    }

    void print2()
    {
        xnet::MutexLockGuard lock(mutex_);
        if (count_ < 10)
        {
            std::cout << "Timer 2: " << count_ << "\n";
            ++count_;

            loop2_->runAfter(1, std::bind(&Printer::print2, this));
        }
        else
        {
            loop2_->quit();
        }
    }

private:
    xnet::MutexLock mutex_;
    xnet::net::EventLoop *loop1_ PT_GUARDED_BY(mutex_);
    xnet::net::EventLoop *loop2_ PT_GUARDED_BY(mutex_);
    int count_ GUARDED_BY(mutex_);
};

int main()
{
    std::unique_ptr<Printer> printer; // make sure printer lives longer than loops, to avoid
                                      // race condition of calling print2() on destructed object.
    xnet::net::EventLoop loop;
    xnet::net::EventLoopThread loopThread;
    xnet::net::EventLoop *loopInAnotherThread = loopThread.startLoop();
    printer.reset(new Printer(&loop, loopInAnotherThread));
    loop.loop();
}
