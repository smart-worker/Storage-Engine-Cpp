#include "bloomfilter.h"
#include <functional>

/**
 * @brief Computes a hash value for a given key using a seed.
 *
 * This function applies a hash function to a given string key and
 * mixes it with a seed to generate different hash values.
 *
 * @param key The input string to be hashed.
 * @param seed An integer seed to vary the hash output.
 * @return A size_t hash value.
 */
inline size_t hash(const std::string &key, int seed)
{
    std::hash<std::string> hasher;
    return hasher(key) ^ (seed * 0x5bd1e995);
}

/**
 * @brief Adds a key to the Bloom filter.
 *
 * This function computes multiple hash values for the given key
 * and sets the corresponding bits in the bit array.
 *
 * @param key The string key to be added to the filter.
 */
void BloomFilter::add(const std::string &key)
{
    for (int i = 0; i < BLOOM_HASH_COUNT; ++i)
    {
        size_t hashValue = hash(key, i) % BLOOM_FILTER_SIZE;
        bitArray.set(hashValue);
    }
}

/**
 * @brief Checks if a key might be in the Bloom filter.
 *
 * This function computes multiple hash values for the given key
 * and checks whether all the corresponding bits are set in the bit array.
 * If all bits are set, the key might be present; otherwise, it is definitely not present.
 *
 * @param key The string key to be checked.
 * @return True if the key might be present, false if it is definitely not present.
 */
bool BloomFilter::mightContain(const std::string &key) const
{
    for (int i = 0; i < BLOOM_HASH_COUNT; ++i)
    {
        size_t hashValue = hash(key, i) % BLOOM_FILTER_SIZE;
        if (!bitArray.test(hashValue))
        {
            return false;
        }
    }
    return true;
}
