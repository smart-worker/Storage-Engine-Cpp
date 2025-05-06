#include "sstable.h"
#include <iostream>
#include <fstream>
#include <filesystem>

/**
 * @brief Writes the SSTable data to a file on disk.
 *
 * Ensures that the parent directory exists before writing. The file is opened
 * in truncation mode, meaning any previous content is overwritten.
 *
 * @param filename The name of the file where SSTable data will be stored.
 * @return True if the write operation was successful, false otherwise.
 */
bool SSTable::writeToDisk(const std::string &filename)
{
    try
    {
        std::filesystem::path filePath(filename);
        std::filesystem::path parentPath = filePath.parent_path();

        if (!parentPath.empty() && !std::filesystem::exists(parentPath))
        {
            std::filesystem::create_directories(parentPath);
        }

        std::ofstream file(filename, std::ios::out | std::ios::trunc);
        if (!file.is_open())
        {
            std::cerr << "Failed to open file for writing: " << filename << std::endl;
            return false;
        }

        for (const auto &entry : data)
        {
            file << entry.first << " " << entry.second << "\n";
        }

        file.close();
        return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Exception while writing SSTable to disk: " << e.what() << std::endl;
        return false;
    }
}

/**
 * @brief Adds an entry to the SSTable and updates the Bloom filter.
 *
 * @param key The key to insert.
 * @param value The corresponding value.
 */
void SSTable::addEntry(const std::string &key, const std::string &value)
{
    data[key] = value;
    bloomFilter.add(key);
}
