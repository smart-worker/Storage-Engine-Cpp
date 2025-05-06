/**
 * @file main.cpp
 * @brief Main entry point for the BLINK DB server application
 */

#include "server/server.h"
#include "benchmarkdata/benchmarkdata.h"
#include "../../part_a/src/StorageEngine/lsmtree.h"
#include <iostream>
#include <string>
#include <stdexcept>

int main()
{
    try
    {
        std::string sstableDir = "sstabledata";

        BenchmarkData data;

        LSMTree store(sstableDir);

        KQueueServer server(store, data);
        return server.run();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
    catch (...)
    {
        std::cerr << "Unknown fatal error occurred" << std::endl;
        return 1;
    }
}
