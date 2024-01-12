#ifndef UTILS_HPP
#define UTILS_HPP

#include "../config/config.hpp"
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "../httpResponse/response.hpp"
#include "Headers.hpp"
#include "Result.hpp"
#include "consts.hpp"

namespace utils
{
Location matchedLocation(const std::string uri, const std::vector<Location> locations);
unsigned int lengthOfPrefixMatch(const std::string string, const std::string pattern);

// string
std::string &trim(std::string &str);
std::string lowerCase(const std::string str);
bool isNumber(const std::string str);
template <typename T> std::string toString(const T value)
{
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

typedef Result<std::string, HttpResponse> FileContentResult;

FileContentResult content(const std::string path);
std::string contentType(const std::string path);

// map
template <typename Key, typename Value> Value value(const std::map<Key, Value> myMap, const Key key)
{
    typename std::map<Key, Value>::const_iterator iterator = myMap.find(key);
    if (iterator != myMap.end())
    {
        return iterator->second;
    }
    else
    {
        return Value();
    }
}

template <typename T> void printError(const T &message)
{
    std::cerr << "\033[31m" << message << "\033[0m" << std::endl;
}

template <class T> std::string to_string(const T &value)
{
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

} // namespace utils

#endif
