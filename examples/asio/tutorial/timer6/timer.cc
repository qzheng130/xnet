#include "xnet/base/Mutex.h"
#include "xnet/net/EventLoop.h"
#include "xnet/net/EventLoopThread.h"

#include <stdio.h>

//
// Minimize locking
//

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

    ~Printer()
    {
        // cout is not thread safe
        // std::cout << "Final count is " << count_ << "\n";
        printf("Final count is %d\n", count_);
    }

    void print1()
    {
        bool shouldQuit = false;
        int count = 0;

        {
            xnet::MutexLockGuard lock(mutex_);
            if (count_ < 10)
            {
                count = count_;
                ++count_;
            }
            else
            {
                shouldQuit = true;
            }
        }

        // out of lock
        if (shouldQuit)
        {
            // printf("loop1_->quit()\n");
            loop1_->quit();
        }
        else
        {
            // cout is not thread safe
            // std::cout << "Timer 1: " << count << "\n";
            printf("Timer 1: %d\n", count);
            loop1_->runAfter(1, std::bind(&Printer::print1, this));
        }
    }

    void print2()
    {
        bool shouldQuit = false;
        int count = 0;

        {
            xnet::MutexLockGuard lock(mutex_);
            if (count_ < 10)
            {
                count = count_;
                ++count_;
            }
            else
            {
                shouldQuit = true;
            }
        }

        // out of lock
        if (shouldQuit)
        {
            // printf("loop2_->quit()\n");
            loop2_->quit();
        }
        else
        {
            // cout is not thread safe
            // std::cout << "Timer 2: " << count << "\n";
            printf("Timer 2: %d\n", count);
            loop2_->runAfter(1, std::bind(&Printer::print2, this));
        }
    }

private:
    xnet::MutexLock mutex_;
    xnet::net::EventLoop *loop1_;
    xnet::net::EventLoop *loop2_;
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
