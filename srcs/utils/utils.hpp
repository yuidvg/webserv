#ifndef UTILS_HPP
#define UTILS_HPP

#include "../config/config.hpp"
#include <cstring>
#include <iostream>
#include <sstream>

#include "consts.hpp"

namespace utils
{
#include "Result.hpp"
Location matchedLocation(const std::string uri, const std::vector<Location> locations);
unsigned int lengthOfPrefixMatch(const std::string string, const std::string pattern);

std::string &trim(std::string &str);
std::string toLower(std::string str);
bool isNumber(const std::string &str);

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

} // namespace utils

#endif
