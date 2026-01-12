#include "examples/procmon/plot.h"
#include "xnet/base/Timestamp.h"
#include <vector>
#include <math.h>
#include <stdio.h>

int main()
{
    std::vector<double> cpu_usage;
    cpu_usage.reserve(300);
    for (int i = 0; i < 300; ++i)
        cpu_usage.push_back(1.0 + sin(pow(i / 30.0, 2)));
    Plot plot(640, 100, 600, 2);
    xnet::Timestamp start(xnet::Timestamp::now());
    const int N = 10000;
    for (int i = 0; i < N; ++i)
        xnet::string png = plot.plotCpu(cpu_usage);
    double elapsed = timeDifference(xnet::Timestamp::now(), start);
    printf("%d plots in %f seconds, %f PNG per second, %f ms per PNG\n", N, elapsed, N / elapsed, elapsed * 1000 / N);
    xnet::string png = plot.plotCpu(cpu_usage);

    FILE *fp = fopen("test.png", "wb");
    fwrite(png.data(), 1, png.size(), fp);
    fclose(fp);
    printf("Image saved to test.png\n");
}
