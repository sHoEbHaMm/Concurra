#include "config.h"

#define MULTITHREADED 1

const unsigned int DATASET = 5000000;

void ProcessDataset(std::array<int, DATASET>& set)
{
    for (int i : set)
    {
        constexpr auto limit = (double)std::numeric_limits<int>::max();
        const auto y = (double)i / limit;
        set[0] += (int)std::sin(std::cos(y) * limit);
    }
}

int main()
{
    /* Threads */
    std::vector<std::thread> workerThreads;

    std::minstd_rand randomEngine;

    std::vector<std::array<int, DATASET>> dataSet{ 4 };

    ChiliTimer timer;

    for (auto& arr : dataSet)
    {
        std::ranges::generate(arr, randomEngine);
    }

#if MULTITHREADED == 1
    for (auto& set : dataSet)
    {
        workerThreads.push_back(std::thread{ ProcessDataset, std::ref(set) });
    }

    timer.Mark();
    for (auto& w : workerThreads)
    {
        w.join();
    }
    auto t = timer.Peek();
    std::cout << "Multithreaded processing datasets took: " << t << "s" << std::endl;
#endif

#if MULTITHREADED == 0
    timer.Mark();
    for (auto& set : dataSet)
    {
        ProcessDataset(set);
    }
    auto t = timer.Peek();
    std::cout << "Non multithreaded processing datasets took: " << t << "s" << std::endl;
#endif

    return 0;
}
