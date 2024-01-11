#ifndef parseConfig_HPP
#define parseConfig_HPP

#include "../utils/utils.hpp"
#include "config.hpp"
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

typedef Result<std::vector<Server>, std::string> ConfigResult;
typedef Result<Server, std::string> ParseServerResult;
typedef Result<Location, std::string> ParseLocationResult;
typedef Result<std::string, std::string> ErrorPageMapResult;

template <typename T> Result<T, std::string> PullWord(std::istringstream &iss);

ParseServerResult ParseServer(std::ifstream &configFile);
ParseLocationResult ParseLocation(std::ifstream &configFile, std::string &locationPath);
ErrorPageMapResult HandleErrorPageDirective(std::istringstream &iss, std::map<int, std::string> &errorPages);
void HandleLocationDirective(std::istringstream &iss, std::ifstream &configFile, Server &server, int type);

ConfigResult parseConfig(const char *configPath);

#endif
