#ifndef PARSE_CONFIG_HPP
#define PARSE_CONFIG_HPP

#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "../Result/Result.hpp"
#include "structure_config.hpp"

#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define YELLO "\x1b[33m"
#define NORMAL "\x1b[0m"

#define FRONT 0
#define BACK 1

typedef Result<std::vector<Server>, std::string> ParseResult;
typedef Result<Server, std::string> ParseServerResult;
typedef Result<std::string, std::string> ParseRoutesResult;
typedef Result<std::string, std::string> ErrorPageMapResult;

template <typename T>
Result<T, std::string> PullWord(std::istringstream &iss);
ParseServerResult ParseServer(std::ifstream &config_file);
ParseRoutesResult ParseLocation(std::ifstream &config_file, Location &location);
ErrorPageMapResult HandleErrorPageDirective(std::istringstream &iss, std::map<int, std::string> &error_page);
void HandleLocationDirective(std::istringstream &iss, std::ifstream &config_file, Server &server, int type);

ParseResult ParseConfig(const char *config_path);

#endif
