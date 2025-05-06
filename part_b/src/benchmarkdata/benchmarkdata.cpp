/**
 * @file benchmarkdata.cpp
 * @brief Implementation of benchmark data generator.
 */

#include "benchmarkdata.h"
#include <iostream>
#include <random>
#include <algorithm>

/**
 * @brief Constructs a BenchmarkData object with the given parameters.
 *
 * Initializes the number of read and write operations, key and value sizes,
 * and generates the necessary test data.
 *
 * @param reads Number of read operations.
 * @param writes Number of write operations.
 * @param keyLength Length of each key.
 * @param valueLength Length of each value.
 */
BenchmarkData::BenchmarkData(
    size_t reads,
    size_t writes,
    size_t keyLength,
    size_t valueLength) : numReads(reads), numWrites(writes),
                          keySize(keyLength), valueSize(valueLength)
{
    std::cout << "Generating test data..." << std::endl;
    generateTestData();
}

/**
 * @brief Generates a random alphanumeric string of a specified length.
 *
 * Uses a random number generator to create a string consisting of
 * uppercase letters, lowercase letters, and digits.
 *
 * @param length Length of the string to generate.
 * @return A randomly generated string of the specified length.
 */
std::string BenchmarkData::generateRandomString(size_t length)
{
    static const char charset[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<int> distribution(0, sizeof(charset) - 2);

    std::string result;
    result.reserve(length);
    for (size_t i = 0; i < length; ++i)
    {
        result += charset[distribution(generator)];
    }

    return result;
}

/**
 * @brief Generates test data consisting of random keys and values.
 *
 * Populates the `keys` vector with randomly generated keys and
 * the `values` vector with randomly generated values.
 */
void BenchmarkData::generateTestData()
{
    keys.reserve(std::max(numReads, numWrites));
    for (size_t i = 0; i < std::max(numReads, numWrites); ++i)
    {
        keys.push_back(generateRandomString(keySize));
    }

    values.reserve(numWrites);
    for (size_t i = 0; i < numWrites; ++i)
    {
        values.push_back(generateRandomString(valueSize));
    }
}