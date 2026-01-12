#include "xnet/base/BlockingQueue.h"
#include "xnet/base/CountDownLatch.h"
#include "xnet/base/Logging.h"
#include "xnet/base/Thread.h"
#include "xnet/base/Timestamp.h"

#include <map>
#include <string>
#include <vector>
#include <stdio.h>
#include <unistd.h>

bool g_verbose = false;

// Many threads, one queue.
class Bench
{
public:
    Bench(int numThreads)
    : latch_(numThreads)
    {
        threads_.reserve(numThreads);
        for (int i = 0; i < numThreads; ++i)
        {
            char name[32];
            snprintf(name, sizeof name, "work thread %d", i);
            threads_.emplace_back(new xnet::Thread(std::bind(&Bench::threadFunc, this), xnet::string(name)));
        }
        for (auto &thr : threads_)
        {
            thr->start();
        }
    }

    void run(int times)
    {
        printf("waiting for count down latch\n");
        latch_.wait();
        LOG_INFO << threads_.size() << " threads started";
        int64_t total_delay = 0;
        for (int i = 0; i < times; ++i)
        {
            xnet::Timestamp now(xnet::Timestamp::now());
            queue_.put(now);
            total_delay += delay_queue_.take();
        }
        printf("Average delay: %.3fus\n", static_cast<double>(total_delay) / times);
    }

    void joinAll()
    {
        for (size_t i = 0; i < threads_.size(); ++i)
        {
            queue_.put(xnet::Timestamp::invalid());
        }

        for (auto &thr : threads_)
        {
            thr->join();
        }
        LOG_INFO << threads_.size() << " threads stopped";
    }

private:
    void threadFunc()
    {
        if (g_verbose)
        {
            printf("tid=%d, %s started\n", xnet::CurrentThread::tid(), xnet::CurrentThread::name());
        }

        std::map<int, int> delays;
        latch_.countDown();
        bool running = true;
        while (running)
        {
            xnet::Timestamp t(queue_.take());
            xnet::Timestamp now(xnet::Timestamp::now());
            if (t.valid())
            {
                int delay = static_cast<int>(timeDifference(now, t) * 1000000);
                // printf("tid=%d, latency = %d us\n",
                //        xnet::CurrentThread::tid(), delay);
                ++delays[delay];
                delay_queue_.put(delay);
            }
            running = t.valid();
        }

        if (g_verbose)
        {
            printf("tid=%d, %s stopped\n", xnet::CurrentThread::tid(), xnet::CurrentThread::name());
            for (const auto &delay : delays)
            {
                printf("tid = %d, delay = %d, count = %d\n", xnet::CurrentThread::tid(), delay.first, delay.second);
            }
        }
    }

    xnet::BlockingQueue<xnet::Timestamp> queue_;
    xnet::BlockingQueue<int> delay_queue_;
    xnet::CountDownLatch latch_;
    std::vector<std::unique_ptr<xnet::Thread>> threads_;
};

int main(int argc, char *argv[])
{
    int threads = argc > 1 ? atoi(argv[1]) : 1;

    Bench t(threads);
    t.run(100000);
    t.joinAll();
}
