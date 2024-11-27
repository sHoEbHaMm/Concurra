#include "config.h"

#define MULTITHREADED 1

const unsigned int DATASET = 50000000;

std::vector<std::array<int, DATASET>> GenerateDatasets()
{
    std::vector<std::array<int, DATASET>> dataSet{ 4 };
    std::minstd_rand randomEngine;

    for (auto& arr : dataSet)
    {
        std::ranges::generate(arr, randomEngine);
    }

    return dataSet;
}

void ProcessDataset(std::span<int> set, int& result)
{
    for (int i : set)
    {
        constexpr auto limit = (double)std::numeric_limits<int>::max();
        const auto y = (double)i / limit;
        result += (int)std::sin(std::cos(y) * limit);
    }
}

int ProcessAllAtOnce_MULTITHREADED()
{
    struct Result {
        int v;
        char padding[60];
    };

    auto dataSet = GenerateDatasets();

    /* Threads */
    std::vector<std::thread> workerThreads;
    ChiliTimer timer;
    Result result[4] = { 0,0,0,0 };
    int total = 0;

    for (size_t i = 0; i < 4; i++)
    {
        workerThreads.push_back(std::thread{ ProcessDataset, std::span{dataSet[i]}, std::ref(result[i].v) });
    }

    timer.Mark();
    for (auto& w : workerThreads)
    {
        w.join();
    }
    auto t = timer.Peek();

    total = result[0].v + result[1].v + result[2].v + result[3].v;
    std::cout << "Multithreaded processing datasets took: " << t << "s" << std::endl;
    std::cout << "Result " << total << std::endl;

    return 0;
}

int ProcessInChunks_MULTITHREADED()
{
    struct Result {
        int v;
        char padding[60];
    };

    auto dataSets = GenerateDatasets();
    const unsigned int subsetSize = DATASET / 1000;
    Result result[4] = { 0,0,0,0 };
    int total = 0;

    std::vector<std::jthread> workerThreads;

    ChiliTimer timer;
    timer.Mark();
    for (size_t i = 0; i < DATASET; i += subsetSize)
    {
        for (size_t j = 0; j < 4; j++)
        {
            workerThreads.push_back(std::jthread{ ProcessDataset, std::span{&dataSets[j][i], subsetSize}, std::ref(result[j].v) });
        }
        workerThreads.clear();
        total = result[0].v + result[1].v + result[2].v + result[3].v;
    }
    auto t = timer.Peek();

    std::cout << "Multithreaded processing datasets IN CHUNKS took: " << t << "s" << std::endl;
    std::cout << "Result " << total << std::endl;

    return 0;
}

int main(int argc, char** argv)
{
    if (argc > 1 && std::string{ argv[1] } == "--chunks")
    {
        return ProcessInChunks_MULTITHREADED();
    }
    return ProcessAllAtOnce_MULTITHREADED();
}