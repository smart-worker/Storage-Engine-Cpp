#ifndef LSM_TREE_H
#define LSM_TREE_H

#include "sstable.h"
#include <vector>
#include <string>
#include <map>
#include "config.h"

/**
 * @file lsmtree.h
 * @brief Header file for the LSM Tree implementation.
 */

/**
 * @brief Log-Structured Merge (LSM) Tree implementation.
 *
 * The LSM Tree maintains an in-memory memtable and persistent SSTables for
 * efficient key-value storage. It supports fast writes and range queries
 * while leveraging Bloom filters for efficient lookups.
 */
class LSMTree
{
private:
    std::map<std::string, std::string> memtable;
    std::vector<SSTable> sstables;
    int sstableCounter = 0;
    std::string sstableDirectory;

    /**
     * @brief Creates the directory for storing SSTables if it does not exist.
     * @return True if the directory is successfully created or already exists, false otherwise.
     */
    bool createSStableDirectory();

    /**
     * @brief Flushes the memtable to SSTables when it reaches its maximum size.
     */
    void flushMemtableToSSTable();

    /**
     * @brief Writes an SSTable to disk and adds it to the list of SSTables.
     * @param sstable The SSTable to be written to disk.
     */
    void writeSSTableToDisk(SSTable &sstable);

public:
    /**
     * @brief Constructs an LSMTree instance with a specified SSTable directory.
     * @param directory The directory where SSTables will be stored.
     */
    LSMTree(const std::string &directory);

    /**
     * @brief Inserts a key-value pair into the LSM Tree.
     * @param key The key to insert.
     * @param value The corresponding value.
     */
    void set(const std::string &key, const std::string &value);

    /**
     * @brief Retrieves the value associated with a given key.
     * @param key The key to look up.
     * @return The associated value if found, otherwise "NOT_FOUND".
     */
    std::string get(const std::string &key);

    /**
     * @brief Marks a key as deleted by inserting a tombstone marker.
     * @param key The key to remove.
     */
    void remove(const std::string &key);

    /**
     * @brief Retrives the values of all the keys stored in the database.
     * @return A vector of values found the database.
     */
    std::vector<std::string> getAllKeyValuePairs();
};

#endif // LSM_TREE_H
