#include "xnet/base/ThreadLocalSingleton.h"
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

void threadFunc(const char *changeTo)
{
    printf("tid=%d, %p name=%s\n", xnet::CurrentThread::tid(), &xnet::ThreadLocalSingleton<Test>::instance(),
           xnet::ThreadLocalSingleton<Test>::instance().name().c_str());
    xnet::ThreadLocalSingleton<Test>::instance().setName(changeTo);
    printf("tid=%d, %p name=%s\n", xnet::CurrentThread::tid(), &xnet::ThreadLocalSingleton<Test>::instance(),
           xnet::ThreadLocalSingleton<Test>::instance().name().c_str());

    // no need to manually delete it
    // xnet::ThreadLocalSingleton<Test>::destroy();
}

int main()
{
    xnet::ThreadLocalSingleton<Test>::instance().setName("main one");
    xnet::Thread t1(std::bind(threadFunc, "thread1"));
    xnet::Thread t2(std::bind(threadFunc, "thread2"));
    t1.start();
    t2.start();
    t1.join();
    printf("tid=%d, %p name=%s\n", xnet::CurrentThread::tid(), &xnet::ThreadLocalSingleton<Test>::instance(),
           xnet::ThreadLocalSingleton<Test>::instance().name().c_str());
    t2.join();

    pthread_exit(0);
}
