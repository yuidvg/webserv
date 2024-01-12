#ifndef parseConfig_HPP
#define parseConfig_HPP

#include "../utils/utils.hpp"
#include "config.hpp"
#include "tokenizeConfig.hpp"
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

typedef utils::Result<std::vector<Server>, std::string> ParseResult;
typedef utils::Result<Server, std::string> ParseServerResult;
typedef utils::Result<Location, std::string> ParseLocationResult;
typedef utils::Result<std::string, std::string> ErrorPageMapResult;

// 設定ファイルを解析するメインの関数
ParseResult ParseConfig(const char *configPath);
ParseLocationResult ParseLocation(std::vector<Tokenize> &tokens, std::string &locationPath);
ErrorPageMapResult HandleErrorPageDirective(std::vector<Tokenize> &tokens, std::map<int, std::string> &errorPages);
void HandleLocationDirective(std::istringstream &iss, std::ifstream &configFile, Server &server, int type);

ParseResult ParseConfig(const char *configPath);

#endif
