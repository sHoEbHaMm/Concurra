#include "config.h"

#define MULTITHREADED 1
#define LOCKGUARD 1

const unsigned int DATASET = 5000000;

void ProcessDataset(std::array<int, DATASET>& set, int& result, std::mutex& mtx)
{
    for (int i : set)
    {
        /* Mutexes come with an overhead where if Thread1 locks the mutex,
        * and another Thread2 is requesting a lock, Thread2 gets stalled 
        * This waiting time might cause performance issues and a single-threaded application
        * might have performed better in this case
        * 
        * So, use mutexes wisely
        */

#if LOCKGUARD == 1
        /* Lockguard advantages :
            1 - Follows RAII, locks mutex upon acquistion and unlocks it when lockguard itself goes out of scope.
            2 - If the operation throws an exception, the mutex still gets unlocked
        */
        std::lock_guard lg{ mtx };
        constexpr auto limit = (double)std::numeric_limits<int>::max();
        const auto y = (double)i / limit;
        result += (int)std::sin(std::cos(y) * limit);

#elif LOCKGUARD == 0
        /* If you forget to unlock mutex after operation, all other threads will get stalled.
        * The program will reach a state termed as Dead Lock.
        */
        mtx.lock();
        constexpr auto limit = (double)std::numeric_limits<int>::max();
        const auto y = (double)i / limit;
        result += (int)std::sin(std::cos(y) * limit);
        mtx.unlock();

#endif
    }
}

int main()
{
    /* Data set */
    std::vector<std::array<int, DATASET>> dataSet{ 4 };
    /* Threads */
    std::vector<std::thread> workerThreads;
    /* Shared variable across threads */
    int result = 0;

    std::minstd_rand randomEngine;

    ChiliTimer timer;

    for (auto& arr : dataSet)
    {
        std::ranges::generate(arr, randomEngine);
    }

#if MULTITHREADED == 1
    std::mutex mtx;

    for (auto& set : dataSet)
    {
        workerThreads.push_back(std::thread{ ProcessDataset, std::ref(set), std::ref(result), std::ref(mtx)});
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
