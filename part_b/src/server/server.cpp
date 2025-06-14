/**
 * @file server.cpp
 * @brief Implementation of KQueue-based server for handling key-value store operations.
 */

#include "server.h"
#include "resp_parser.h"
#include <iostream>
#include <sys/event.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <algorithm>
#include <random>

/**
 * @brief Constructs a KQueueServer object.
 * @param store Reference to the LSMTree storage engine.
 * @param data Reference to the BenchmarkData generator.
 */
KQueueServer::KQueueServer(LSMTree &store, BenchmarkData &data)
    : store(store), data(data), server_fd(-1), kq(-1)
{
}

/**
 * @brief Destroys the KQueueServer object and closes open file descriptors.
 */
KQueueServer::~KQueueServer()
{
    if (server_fd >= 0)
    {
        close(server_fd);
    }
    if (kq >= 0)
    {
        close(kq);
    }
}

void KQueueServer::sendUpdateNotification()
{
    const std::string message = "UPDATE";

    if (subscriptions.size() > 0)
    {
        // This is the message format subscribers will receive
        // Format: ["message", "channel_name", "the_message"]
        std::string message_to_send = RespParser::createResponseForSubscriber(message, channel_name);

        for (const int &fd : subscriptions)
        {
            send(fd, message_to_send.c_str(), message_to_send.length(), 0);
        }
    }
}

/**
 * @brief Processes a client command and generates a response.
 * @param args Parsed command arguments.
 * @param rn Random index for benchmark data access.
 * @return A RESP-formatted response string.
 */
std::string KQueueServer::processCommand(const std::vector<std::string> &args, int rn, int fd)
{
    if (args.empty())
        return RespParser::createError("no command");

    std::string cmd = args[0];
    std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::tolower);
    std::cout << "Command: " << cmd << std::endl;

    try
    {
        if (cmd == "subscribe")
        {
            // SUBSCRIBE channel1 [channel2 ...]
            for (size_t i = 1; i < args.size(); ++i)
            {
                subscriptions.push_back(fd);

                // IMPORTANT: Send confirmation back to the client in RESP format
                // This is what Redis clients like ioredis expect.
                // Format: ["subscribe", "channel_name", 1]
                std::string response = "*3\r\n$9\r\nsubscribe\r\n$" +
                                       std::to_string(channel_name.length()) + "\r\n" + channel_name + "\r\n" +
                                       ":1\r\n";
                return response;
            }
        }
        else if (cmd == "publish" && args.size() >= 2)
        {
            const std::string &message = args[1];

            int subscribers_count = 0;
            if (subscriptions.size() > 0)
            {
                // This is the message format subscribers will receive
                // Format: ["message", "channel_name", "the_message"]
                std::string message_to_send = RespParser::createResponseForSubscriber(message, channel_name);

                for (const int &fd : subscriptions)
                {
                    send(fd, message_to_send.c_str(), message_to_send.length(), 0);
                    subscribers_count++;
                }
            }
            // Respond to the publisher with the count of subscribers reached
            std::string response = ":" + std::to_string(subscribers_count) + "\r\n";
            return response;
        }
        if (cmd == "getall" && args.size() == 1)
        {
            std::vector<std::string> arrVals = store.getAllKeyValuePairs();
            return RespParser::serializeArray(arrVals);
        }
        else if (cmd == "set" && args.size() == 3)
        {
            // store.set(data.keys[rn], data.values[rn]); // For benchmark
            store.set(args[1], args[2]);
            sendUpdateNotification();
            return RespParser::createSimpleString("OK");
        }
        else if (cmd == "get" && args.size() == 2)
        {
            // std::string value = store.get(data.keys[rn]); // For benchmark
            std::string value = store.get(args[1]);
            return RespParser::serializeBulkString(value.length() ? value : "NULL");
        }
        else if (cmd == "del" && args.size() == 2)
        {
            store.remove(args[1]);
            sendUpdateNotification();
            return RespParser::createSimpleString("OK");
        }
        else if (cmd == "ping")
        {
            if (args.size() == 1)
                return RespParser::createSimpleString(args[0]); // PING msg
            else
                return RespParser::createSimpleString("PONG"); // Default PING
        }
        else if (cmd == "echo" && args.size() == 1)
        {
            return RespParser::serializeBulkString(args[0]);
        }
        else if (cmd == "command")
        {
            return "*0\r\n"; // Respond with empty array
        }
        else if (cmd == "select" && args.size() == 1)
        {
            return RespParser::createSimpleString("OK"); // Stubbed; no DB switching
        }
        else if (cmd == "client")
        {
            return RespParser::createSimpleString("OK"); // Ignore subcommands
        }
        else if (cmd == "info")
        {
            return RespParser::serializeBulkString("redis_version: 6.0.0\r\n");
        }
        else
            return RespParser::createError("unknown command");
    }
    catch (const std::exception &e)
    {
        std::cerr << "Unknown command received: " << cmd << std::endl;
        return RespParser::createError(e.what());
    }
}

/**
 * @brief Handles client requests.
 * @param fd Client socket file descriptor.
 * @param rn Random index for benchmark data access.
 */
void KQueueServer::handleClient(int fd, int rn)
{
    char buffer[BUFFER_SIZE] = {0};
    ssize_t bytes_read = recv(fd, buffer, BUFFER_SIZE, 0);

    if (bytes_read > 0)
    {
        // std::cout << "RAW CLIENT INPUT:\n"
        //           << buffer << std::endl;
        auto args = RespParser::parseArray(std::string(buffer, bytes_read));
        std::string response = processCommand(args, rn, fd);
        send(fd, response.c_str(), response.size(), 0);
    }
}

/**
 * @brief Runs the KQueue-based event-driven server.
 * @return 0 on successful execution, 1 on error.
 */
int KQueueServer::run()
{
    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0)
    {
        std::cerr << "Failed to create socket" << std::endl;
        return 1;
    }

    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int)) < 0)
    {
        std::cerr << "Failed to set socket options" << std::endl;
        return 1;
    }

    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_port = htons(PORT),
        .sin_addr.s_addr = INADDR_ANY};

    if (bind(server_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        std::cerr << "Failed to bind socket" << std::endl;
        return 1;
    }

    if (listen(server_fd, SOMAXCONN) < 0)
    {
        std::cerr << "Failed to listen on socket" << std::endl;
        return 1;
    }

    kq = kqueue();
    if (kq < 0)
    {
        std::cerr << "Failed to create kqueue" << std::endl;
        return 1;
    }

    struct kevent changes[1];
    EV_SET(&changes[0], server_fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
    if (kevent(kq, changes, 1, NULL, 0, NULL) < 0)
    {
        std::cerr << "Failed to add server socket to kqueue" << std::endl;
        return 1;
    }

    std::cout << "kqueue server listening on port " << PORT << std::endl;

    while (true)
    {
        struct kevent events[MAX_EVENTS];
        int nev = kevent(kq, NULL, 0, events, MAX_EVENTS, NULL);

        if (nev < 0)
        {
            std::cerr << "kevent error" << std::endl;
            break;
        }

        for (int i = 0; i < nev; i++)
        {
            int fd = (int)events[i].ident;

            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<int> distrib(0, 100000);
            int random_number = distrib(gen);

            if (fd == server_fd)
            {
                struct sockaddr_in client_addr;
                socklen_t len = sizeof(client_addr);
                int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &len);

                if (client_fd < 0)
                {
                    std::cerr << "Failed to accept connection" << std::endl;
                    continue;
                }

                struct kevent event;
                EV_SET(&event, client_fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
                kevent(kq, &event, 1, NULL, 0, NULL);
            }
            else
            {
                handleClient(fd, random_number);

                if (events[i].flags & EV_EOF)
                {
                    close(fd);
                }
            }
        }
    }

    return 0;
}
