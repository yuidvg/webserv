#ifndef parseConfig_HPP
#define parseConfig_HPP

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
typedef utils::Result<std::string, std::string> StringResult;

// 設定ファイルを解析するメインの関数
ParseResult parseConfig(const char *configPath);
ParseServerResult parseServerContext(std::vector<std::string> &tokens);
ParseLocationResult parseLocationContext(std::vector<std::string> &tokens, std::string &locationPath);
void removeSemicolonFromToken(std::vector<std::string> &token);

#endif
