/**
 * @file resp_parser.h
 * @brief Redis RESP-2 protocol parser implementation header file
 */

#ifndef RESP_PARSER_H
#define RESP_PARSER_H

#include <string>
#include <vector>

/**
 * @class RespParser
 * @brief Parser for Redis RESP-2 protocol
 *
 * Handles serialization and deserialization of Redis RESP-2 protocol messages.
 */
class RespParser
{
public:
    /**
     * @brief Parses a RESP array message into command arguments
     * @param buffer The RESP message buffer
     * @return Vector of command arguments
     */
    static std::vector<std::string> parseArray(const std::string &buffer);

    /**
     * @brief Serializes a string into RESP bulk string format
     * @param value The string to serialize
     * @return RESP formatted bulk string
     */
    static std::string serializeBulkString(const std::string &value);

    /**
     * @brief Creates a RESP simple string (status) response
     * @param status The status message
     * @return RESP formatted simple string
     */
    static std::string createSimpleString(const std::string &status);

    /**
     * @brief Creates a RESP error response
     * @param error The error message
     * @return RESP formatted error message
     */
    static std::string createError(const std::string &error);
};

#endif // RESP_PARSER_H
