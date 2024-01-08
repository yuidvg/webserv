#ifndef UTILS_HPP
#define UTILS_HPP

#include "../config/config.hpp"
#include <cstring>
#include <iostream>
#include <sstream>

namespace utils
{
const Location matchedLocation(const std::string uri, const std::vector<Location> routes);
const unsigned int lengthOfPrefixMatch(const std::string string, const std::string pattern);

std::string &trim(std::string &str);
std::string toLower(std::string str);
bool isNumber(const std::string &str);

} // namespace utils

#endif
