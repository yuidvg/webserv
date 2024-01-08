#ifndef UTILS_HPP
#define UTILS_HPP

namespace utils
{
const Route matchedRoute(const std::string uri, const std::vector<Route> routes);
const unsigned int lengthOfPrefixMatch(const std::string string, const std::string pattern);
} // namespace utils

#endif
