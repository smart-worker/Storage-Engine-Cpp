/**
 * @file resp_parser.cpp
 * @brief Implementation of Redis RESP-2 protocol parser.
 */

#include "resp_parser.h"
#include <sstream>

/**
 * @brief Parses a RESP-2 array message and extracts command arguments.
 *
 * RESP-2 arrays start with '*' followed by the number of elements, and each element
 * is a bulk string prefixed with '$' followed by its length.
 *
 * @param buffer The RESP-2 formatted string received from the client.
 * @return A vector containing the parsed arguments as strings.
 */
std::vector<std::string> RespParser::parseArray(const std::string &buffer)
{
    std::vector<std::string> args;
    std::istringstream stream(buffer);
    char prefix;
    int arg_count;

    stream >> prefix >> arg_count;
    stream.ignore(2, '\n'); // Skip \r\n
    for (int i = 0; i < arg_count; ++i)
    {
        stream >> prefix; // Should be '$'
        int length;
        stream >> length;
        stream.ignore(2, '\n'); // Skip \r\n
        std::string arg(length, '\0');
        stream.read(&arg[0], length);
        args.push_back(arg);
        stream.ignore(2, '\n'); // Skip \r\n
    }
    return args;
}

/**
 * @brief Serializes a string into a RESP-2 bulk string format.
 *
 * If the string is empty, it returns the RESP-2 null bulk string "$-1\r\n".
 *
 * @param value The string to be serialized.
 * @return A RESP-2 formatted bulk string.
 */
std::string RespParser::serializeBulkString(const std::string &value)
{
    if (value.empty())
        return "$-1\r\n";
    return "$" + std::to_string(value.length()) + "\r\n" + value + "\r\n";
}

/**
 * @brief Creates a RESP-2 simple string response.
 *
 * Simple strings are used for success messages and start with '+'.
 *
 * @param status The success message to return.
 * @return A RESP-2 formatted simple string.
 */
std::string RespParser::createSimpleString(const std::string &status)
{
    return "+" + status + "\r\n";
}

/**
 * @brief Creates a RESP-2 error response.
 *
 * Error messages start with '-' and contain an error description.
 *
 * @param error The error message.
 * @return A RESP-2 formatted error response.
 */
std::string RespParser::createError(const std::string &error)
{
    return "-ERR " + error + "\r\n";
}

/**
 * @brief Serializes a vector of strings into a RESP-2 array of bulk strings.
 *
 * This function constructs the array prefix '*' followed by the number of elements.
 * It then iterates through the vector, serializing each string into a
 * RESP-2 bulk string format using the serializeBulkString method.
 *
 * @param values The vector of strings to be serialized.
 * @return A single RESP-2 formatted string representing the array.
 */
std::string RespParser::serializeArray(const std::vector<std::string> &values)
{
    std::stringstream ss;
    // 1. Write the array prefix and element count
    ss << "*" << values.size() << "\r\n";

    // 2. Append each element as a serialized bulk string
    for (const auto &value : values)
    {
        ss << serializeBulkString(value);
    }

    return ss.str();
}