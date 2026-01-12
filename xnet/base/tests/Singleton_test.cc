#include "xnet/base/Singleton.h"
#include "xnet/base/CurrentThread.h"
#include "xnet/base/Thread.h"

#include <stdio.h>

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

class TestNoDestroy : xnet::noncopyable
{
public:
    // Tag member for Singleton<T>
    void no_destroy();

    TestNoDestroy() { printf("tid=%d, constructing TestNoDestroy %p\n", xnet::CurrentThread::tid(), this); }

    ~TestNoDestroy() { printf("tid=%d, destructing TestNoDestroy %p\n", xnet::CurrentThread::tid(), this); }
};

void threadFunc()
{
    printf("tid=%d, %p name=%s\n", xnet::CurrentThread::tid(), &xnet::Singleton<Test>::instance(),
           xnet::Singleton<Test>::instance().name().c_str());
    xnet::Singleton<Test>::instance().setName("only one, changed");
}

int main()
{
    xnet::Singleton<Test>::instance().setName("only one");
    xnet::Thread t1(threadFunc);
    t1.start();
    t1.join();
    printf("tid=%d, %p name=%s\n", xnet::CurrentThread::tid(), &xnet::Singleton<Test>::instance(),
           xnet::Singleton<Test>::instance().name().c_str());
    xnet::Singleton<TestNoDestroy>::instance();
    printf("with valgrind, you should see %zd-byte memory leak.\n", sizeof(TestNoDestroy));
}
