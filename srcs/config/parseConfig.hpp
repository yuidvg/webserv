#ifndef parseConfig_HPP
#define parseConfig_HPP

#include "config.hpp"
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

typedef utils::Result<std::vector<Server>, std::string> ParseResult;
typedef utils::Result<Server, std::string> ParseServerResult;
typedef utils::Result<std::string, std::string> ParseRoutesResult;
typedef utils::Result<std::string, std::string> ErrorPageMapResult;

template <typename T> utils::Result<T, std::string> PullWord(std::istringstream &iss);

ParseServerResult ParseServer(std::ifstream &config_file);
ParseRoutesResult ParseLocation(std::ifstream &config_file, Location &location);
ErrorPageMapResult HandleErrorPageDirective(std::istringstream &iss, std::map<int, std::string> &errorPages);
void HandleLocationDirective(std::istringstream &iss, std::ifstream &config_file, Server &server, int type);

ParseResult ParseConfig(const char *config_path);

#endif
