#pragma once
#include "../webserv.hpp"

std::string resolvePath(const std::string &path, const Location &location);
std::string resolvePath(const std::string &path, const std::string &location);
MatchedServerResult matchedServer(const std::string &host, const Servers &servers, const Connection &sd);
Location getLocation(const std::string &target, const std::vector<Location> &locations);
