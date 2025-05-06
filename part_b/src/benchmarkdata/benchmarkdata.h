/**
 * @file benchmarkdata.h
 * @brief Benchmark data generator for LSM Tree testing
 */

#ifndef BENCHMARK_DATA_H
#define BENCHMARK_DATA_H

#include <string>
#include <vector>

/**
 * @class BenchmarkData
 * @brief Generates and stores test data for benchmarking
 *
 * This class pre-generates random keys and values for consistent
 * benchmark testing of storage engines.
 */
class BenchmarkData
{
private:
    size_t numReads;
    size_t numWrites;
    size_t keySize;
    size_t valueSize;

    /**
     * @brief Generates a random string of specified length
     * @param length The length of the string to generate
     * @return A random string
     */
    std::string generateRandomString(size_t length);

    /**
     * @brief Generates all test data (keys and values)
     */
    void generateTestData();

public:
    std::vector<std::string> keys;
    std::vector<std::string> values;

    /**
     * @brief Constructor
     * @param reads Number of read operations (default: 1,000,000)
     * @param writes Number of write operations (default: 1,000,000)
     * @param keyLength Length of each key (default: 16)
     * @param valueLength Length of each value (default: 16)
     */
    BenchmarkData(
        size_t reads = 1000000,
        size_t writes = 1000000,
        size_t keyLength = 16,
        size_t valueLength = 16);
};

#endif // BENCHMARK_DATA_H
