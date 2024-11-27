#include "config.h"
#include "Dataset.h"
#include "Processing.h"

int main(int argc, char** argv)
{
    if (argc > 1 && std::string{ argv[1] } == "--chunks")
    {
        return ProcessInChunks_MULTITHREADED();
    }
    return ProcessAllAtOnce_MULTITHREADED();
}