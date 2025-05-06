#ifndef SSTABLE_H
#define SSTABLE_H

#include "bloomfilter.h"
#include <map>
#include <string>
#include "config.h"

/**
 * @file sstable.h
 * @brief Header file defining the SSTable class.
 */

/**
 * @brief Represents an SSTable (Sorted String Table) in the LSM tree.
 *
 * An SSTable is a persistent, immutable key-value store used in LSM trees.
 * It maintains a sorted map of key-value pairs and a Bloom filter for fast lookups.
 */
class SSTable
{
public:
    BloomFilter bloomFilter;
    std::map<std::string, std::string> data;

    /**
     * @brief Writes the SSTable data to a file on disk.
     *
     * @param filename The name of the file where the SSTable data will be stored.
     * @return True if the write operation was successful, false otherwise.
     */
    bool writeToDisk(const std::string &filename);

    /**
     * @brief Adds an entry to the SSTable and updates the Bloom filter.
     *
     * @param key The key to insert.
     * @param value The corresponding value.
     */
    void addEntry(const std::string &key, const std::string &value);
};

#endif // SSTABLE_H