#pragma once
#include "../webserv.hpp"

std::string comply(const std::string &path, const Location &location);
MatchedServerResult matchedServer(const std::string &host, const Servers &servers, const Socket &sd);
Location matchedLocation(const std::string &target, const std::vector<const Location> &locations);
