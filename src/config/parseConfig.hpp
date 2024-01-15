#ifndef parseConfig_HPP
#define parseConfig_HPP

#include "config.hpp"
#include "../utils/utils.hpp"
#include "parseDirective.hpp"
#include "tokenizeConfig.hpp"
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

typedef Result<std::vector<Server>, std::string> ConfigResult;
typedef Result<Server, std::string> ParseServerResult;
typedef Result<Location, std::string> ParseLocationResult;
typedef Result<std::string, std::string> StringResult;

// 設定ファイルを解析するメインの関数
ConfigResult parseConfig(const char *configPath);
ParseServerResult parseServerContext(std::vector<std::string> &tokens);
ParseLocationResult parseLocationContext(std::vector<std::string> &tokens, std::string &locationPath);
void removeSemicolonFromToken(std::vector<std::string> &token);


#endif
