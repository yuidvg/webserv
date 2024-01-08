#ifndef parseConfig_HPP
#define parseConfig_HPP

#include "../Result/Result.hpp"
#include "config.hpp"
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

typedef Result<std::vector<Server>, std::string> ParseResult;
typedef Result<Server, std::string> ParseServerResult;
typedef Result<std::string, std::string> ParseRoutesResult;
typedef Result<std::string, std::string> ErrorPageMapResult;

template <typename T> Result<T, std::string> PullWord(std::istringstream &iss);
ParseServerResult ParseServer(std::ifstream &config_file);
ParseRoutesResult ParseLocation(std::ifstream &config_file, Location &location);
ErrorPageMapResult HandleErrorPageDirective(std::istringstream &iss, std::map<int, std::string> &errorPages);
void HandleLocationDirective(std::istringstream &iss, std::ifstream &config_file, Server &server, int type);

ParseResult ParseConfig(const char *config_path);

#endif
