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

// 設定ファイルを解析するメインの関数
ParseResult ParseConfig(const char *configPath);
ParseServerResult ParseServer(std::vector<Tokenize> &tokens);
ParseLocationResult ParseLocation(std::vector<Tokenize> &tokens, std::string &locationPath);

#endif
