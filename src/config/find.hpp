#ifndef CONFIG_FIND_HPP
#define CONFIG_FIND_HPP
#include "../webserv.hpp"

MatchedServerResult matchedServer(const std::string &host, const Servers &servers, const Sd &sd);
Location matchedLocation(const std::string &target, const std::vector<const Location> &locations);

#endif // CONFIG_FIND_HPP
