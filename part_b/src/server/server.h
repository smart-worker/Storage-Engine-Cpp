/**
 * @file server.h
 * @brief KQueue-based server for LSM Tree storage engine
 */

#ifndef SERVER_H
#define SERVER_H

#include <string>
#include <vector>
#include "../../part_a/src/StorageEngine/lsmtree.h"
#include "../benchmarkdata/benchmarkdata.h"

#define PORT 9002
#define MAX_EVENTS 1024
#define BUFFER_SIZE 1024

/**
 * @class KQueueServer
 * @brief Server implementation using kqueue for I/O multiplexing
 *
 * Handles client connections and processes Redis-compatible commands
 * using the LSM Tree storage engine.
 */
class KQueueServer
{
private:
    int server_fd;
    int kq;
    LSMTree &store;
    BenchmarkData &data;

    /**
     * @brief Processes a command from a client
     * @param args Command arguments
     * @param rn Random number for benchmark data access
     * @return Response to send to client
     */
    std::string processCommand(const std::vector<std::string> &args, int rn);

    /**
     * @brief Handles a client connection
     * @param fd Client socket file descriptor
     * @param rn Random number for benchmark data access
     */
    void handleClient(int fd, int rn);

public:
    /**
     * @brief Constructor
     * @param store LSM Tree storage engine
     * @param data Benchmark data
     */
    KQueueServer(LSMTree &store, BenchmarkData &data);

    /**
     * @brief Destructor
     */
    ~KQueueServer();

    /**
     * @brief Initializes and starts the server
     * @return 0 on success, error code otherwise
     */
    int run();
};

#endif // SERVER_H
