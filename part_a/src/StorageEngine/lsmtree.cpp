#include "lsmtree.h"
#include <iostream>
#include <filesystem>

/**
 * @brief Directory to store SSTable files.
 */
const std::string SSTABLE_DIRECTORY = "sstabledata";

/**
 * @brief Constructs an LSMTree instance with a specified SSTable directory.
 *
 * Ensures the directory path ends with a separator for consistency.
 *
 * @param directory The directory where SSTables will be stored.
 */
LSMTree::LSMTree(const std::string &directory) : sstableDirectory(directory)
{
    if (!sstableDirectory.empty() && sstableDirectory.back() != '/')
    {
        sstableDirectory += '/';
    }
}

/**
 * @brief Creates the directory for storing SSTables if it does not exist.
 *
 * @return True if the directory was created or already exists, false on failure.
 */
bool LSMTree::createSStableDirectory()
{
    try
    {
        if (!std::filesystem::exists(SSTABLE_DIRECTORY))
        {
            return std::filesystem::create_directories(SSTABLE_DIRECTORY);
        }
        return true;
    }
    catch (const std::filesystem::filesystem_error &e)
    {
        std::cerr << "Filesystem error creating directory: " << e.what() << std::endl;
        return false;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error creating directory: " << e.what() << std::endl;
        return false;
    }
}

/**
 * @brief Inserts a key-value pair into the LSM Tree.
 *
 * If the memtable reaches its maximum size, it is flushed to an SSTable.
 *
 * @param key The key to insert.
 * @param value The corresponding value.
 */
void LSMTree::set(const std::string &key, const std::string &value)
{
    memtable[key] = value;

    if (memtable.size() >= MAX_MEMTABLE_SIZE)
    {
        flushMemtableToSSTable();
    }
}

/**
 * @brief Retrieves the value associated with a given key.
 *
 * The lookup first checks the memtable, then searches SSTables if necessary.
 *
 * @param key The key to look up.
 * @return The associated value if found, otherwise "NOT_FOUND".
 */
std::string LSMTree::get(const std::string &key)
{
    std::cout << "KEY IS: " << key << std::endl;
    auto it = memtable.find(key);
    if (it != memtable.end())
    {
        return it->second;
    }

    for (const auto &it : memtable)
    {
        std::cout << "Key: " << it.first << " Value: " << it.second << std::endl;
    }

    for (const auto &sstable : sstables)
    {
        if (sstable.bloomFilter.mightContain(key))
        {
            auto sstableIt = sstable.data.find(key);
            if (sstableIt != sstable.data.end())
            {
                return sstableIt->second;
            }
        }
    }
    return "NOT_FOUND";
}

/**
 * @brief Marks a key as deleted by inserting a tombstone marker.
 *
 * If the memtable reaches its maximum size, it is flushed to an SSTable.
 *
 * @param key The key to remove.
 */
void LSMTree::remove(const std::string &key)
{
    memtable[key] = "DELETED";

    if (memtable.size() >= MAX_MEMTABLE_SIZE)
    {
        flushMemtableToSSTable();
    }
}

/**
 * @brief Flushes the memtable to SSTables on disk.
 *
 * The memtable entries are sorted and split into multiple SSTables
 * if the data size exceeds the maximum SSTable limit.
 */
void LSMTree::flushMemtableToSSTable()
{
    SSTable newSSTable;

    for (const auto &entry : memtable)
    {
        newSSTable.addEntry(entry.first, entry.second);

        if (newSSTable.data.size() >= MAX_SSTABLE_SIZE)
        {
            writeSSTableToDisk(newSSTable);
            newSSTable.data.clear();
        }
    }

    if (!newSSTable.data.empty())
    {
        writeSSTableToDisk(newSSTable);
    }

    memtable.clear();
}

/**
 * @brief Writes an SSTable to disk and adds it to the list of SSTables.
 *
 * @param sstable The SSTable to be persisted.
 */
void LSMTree::writeSSTableToDisk(SSTable &sstable)
{
    std::string filename = SSTABLE_DIRECTORY + "/sstable_" + std::to_string(sstableCounter++) + ".txt";
    sstable.writeToDisk(filename);
    sstables.push_back(sstable);
}