#include "xnet/base/Singleton.h"
#include "xnet/base/CurrentThread.h"
#include "xnet/base/ThreadLocal.h"
#include "xnet/base/Thread.h"

#include <stdio.h>
#include <unistd.h>

class Test : xnet::noncopyable
{
public:
    Test() { printf("tid=%d, constructing %p\n", xnet::CurrentThread::tid(), this); }

    ~Test() { printf("tid=%d, destructing %p %s\n", xnet::CurrentThread::tid(), this, name_.c_str()); }

    const xnet::string &name() const { return name_; }

    void setName(const xnet::string &n) { name_ = n; }

private:
    xnet::string name_;
};

#define STL xnet::Singleton<xnet::ThreadLocal<Test>>::instance().value()

void print() { printf("tid=%d, %p name=%s\n", xnet::CurrentThread::tid(), &STL, STL.name().c_str()); }

void threadFunc(const char *changeTo)
{
    print();
    STL.setName(changeTo);
    sleep(1);
    print();
}

int main()
{
    STL.setName("main one");
    xnet::Thread t1(std::bind(threadFunc, "thread1"));
    xnet::Thread t2(std::bind(threadFunc, "thread2"));
    t1.start();
    t2.start();
    t1.join();
    print();
    t2.join();
    pthread_exit(0);
}
