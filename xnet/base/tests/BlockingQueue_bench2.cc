#include "xnet/base/BlockingQueue.h"
#include "xnet/base/CountDownLatch.h"
#include "xnet/base/Thread.h"
#include "xnet/base/Timestamp.h"

#include <map>
#include <string>
#include <vector>
#include <stdio.h>
#include <unistd.h>

// hot potato benchmarking https://en.wikipedia.org/wiki/Hot_potato
// N threads, one hot potato.
class Bench
{
public:
    Bench(int numThreads)
    : startLatch_(numThreads)
    , stopLatch_(1)
    {
        queues_.reserve(numThreads);
        threads_.reserve(numThreads);
        for (int i = 0; i < numThreads; ++i)
        {
            queues_.emplace_back(new xnet::BlockingQueue<int>());
            char name[32];
            snprintf(name, sizeof name, "work thread %d", i);
            threads_.emplace_back(new xnet::Thread([this, i] { threadFunc(i); }, xnet::string(name)));
        }
    }

    void Start()
    {
        xnet::Timestamp start = xnet::Timestamp::now();
        for (auto &thr : threads_)
        {
            thr->start();
        }
        startLatch_.wait();
        xnet::Timestamp started = xnet::Timestamp::now();
        printf("all %zd threads started, %.3fms\n", threads_.size(), 1e3 * timeDifference(started, start));
    }

    void Run()
    {
        xnet::Timestamp start = xnet::Timestamp::now();
        const int rounds = 100003;
        queues_[0]->put(rounds);

        auto done = done_.take();
        double elapsed = timeDifference(done.second, start);
        printf("thread id=%d done, total %.3fms, %.3fus / round\n", done.first, 1e3 * elapsed, 1e6 * elapsed / rounds);
    }

    void Stop()
    {
        xnet::Timestamp stop = xnet::Timestamp::now();
        for (const auto &queue : queues_)
        {
            queue->put(-1);
        }
        for (auto &thr : threads_)
        {
            thr->join();
        }

        xnet::Timestamp t2 = xnet::Timestamp::now();
        printf("all %zd threads joined, %.3fms\n", threads_.size(), 1e3 * timeDifference(t2, stop));
    }

private:
    void threadFunc(int id)
    {
        startLatch_.countDown();

        xnet::BlockingQueue<int> *input = queues_[id].get();
        xnet::BlockingQueue<int> *output = queues_[(id + 1) % queues_.size()].get();
        while (true)
        {
            int value = input->take();
            if (value > 0)
            {
                output->put(value - 1);
                if (verbose_)
                {
                    // printf("thread %d, got %d\n", id, value);
                }
                continue;
            }

            if (value == 0)
            {
                done_.put(std::make_pair(id, xnet::Timestamp::now()));
            }
            break;
        }
    }

    using TimestampQueue = xnet::BlockingQueue<std::pair<int, xnet::Timestamp>>;
    TimestampQueue done_;
    xnet::CountDownLatch startLatch_, stopLatch_;
    std::vector<std::unique_ptr<xnet::BlockingQueue<int>>> queues_;
    std::vector<std::unique_ptr<xnet::Thread>> threads_;
    const bool verbose_ = true;
};

int main(int argc, char *argv[])
{
    int threads = argc > 1 ? atoi(argv[1]) : 1;

    printf("sizeof BlockingQueue = %zd\n", sizeof(xnet::BlockingQueue<int>));
    printf("sizeof deque<int> = %zd\n", sizeof(std::deque<int>));
    Bench t(threads);
    t.Start();
    t.Run();
    t.Stop();
    // exit(0);
}
