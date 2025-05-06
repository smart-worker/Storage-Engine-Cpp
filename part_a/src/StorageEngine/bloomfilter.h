#ifndef BLOOM_FILTER_H
#define BLOOM_FILTER_H

#include <string>
#include <bitset>
#include "config.h"

/**
 * @brief Computes a hash value for a given key using a seed.
 *
 * This function generates a hash value by applying a hash function to
 * the input key and modifying it with a seed value.
 *
 * @param key The string key to be hashed.
 * @param seed An integer seed to vary the hash output.
 * @return A size_t hash value.
 */
inline size_t hash(const std::string &key, int seed);

/**
 * @brief Bloom Filter implementation for probabilistic membership testing.
 *
 * The Bloom Filter uses multiple hash functions to store elements in a bit array.
 * It allows fast membership queries with a possibility of false positives but no false negatives.
 */
class BloomFilter
{
private:
    std::bitset<BLOOM_FILTER_SIZE> bitArray;

public:
    /**
     * @brief Adds a key to the Bloom filter.
     *
     * This function hashes the given key multiple times and sets the corresponding
     * bits in the bit array.
     *
     * @param key The string key to be added.
     */
    void add(const std::string &key);

    /**
     * @brief Checks if a key might be present in the Bloom filter.
     *
     * This function hashes the key and checks whether all the corresponding
     * bits are set in the bit array. If all bits are set, the key might be present;
     * otherwise, it is definitely not present.
     *
     * @param key The string key to be checked.
     * @return True if the key might be present, false if it is definitely not present.
     */
    bool mightContain(const std::string &key) const;
};

#endif // BLOOM_FILTER_H
