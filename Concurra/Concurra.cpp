#include "config.h"
#include "Processing.h"

int main(int argc, char** argv)
{
    if (argc > 1 && std::string{ argv[1] } == "--good")
    {
        return GoodProcessInChunks();
    }
    return BadProcessInChunks();
}