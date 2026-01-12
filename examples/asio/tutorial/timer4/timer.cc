#include "xnet/net/EventLoop.h"

#include <iostream>

class Printer : xnet::noncopyable
{
public:
    Printer(xnet::net::EventLoop *loop)
    : loop_(loop)
    , count_(0)
    {
        // Note: loop.runEvery() is better for this use case.
        loop_->runAfter(1, std::bind(&Printer::print, this));
    }

    ~Printer() { std::cout << "Final count is " << count_ << "\n"; }

    void print()
    {
        if (count_ < 5)
        {
            std::cout << count_ << "\n";
            ++count_;

            loop_->runAfter(1, std::bind(&Printer::print, this));
        }
        else
        {
            loop_->quit();
        }
    }

private:
    xnet::net::EventLoop *loop_;
    int count_;
};

int main()
{
    xnet::net::EventLoop loop;
    Printer printer(&loop);
    loop.loop();
}
