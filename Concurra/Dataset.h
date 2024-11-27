#pragma once
#include "config.h"

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
