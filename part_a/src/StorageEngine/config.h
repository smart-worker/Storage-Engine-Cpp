#ifndef CONFIG_H
#define CONFIG_H

/**
 * @file config.h
 * @brief Configuration file for LSM Tree and Bloom Filter settings.
 */

/**
 * @brief Maximum number of key-value pairs allowed in the memtable before flushing to disk.
 */
#define MAX_MEMTABLE_SIZE 10000

/**
 * @brief Maximum number of key-value pairs stored in each SSTable.
 */
#define MAX_SSTABLE_SIZE 10000

/**
 * @brief Size of the Bloom filter's bit array.
 */
#define BLOOM_FILTER_SIZE 200000

/**
 * @brief Number of hash functions used in the Bloom filter.
 */
#define BLOOM_HASH_COUNT 9

#endif // CONFIG_H
