#ifndef parseConfig_HPP
#define parseConfig_HPP

#include "../types/all.hpp"
#include "parseDirective.hpp"

ConfigResult parseConfig(const char *configPath);
ParseServerResult parseServerContext(std::vector<std::string> &tokens);
ParseLocationResult parseLocationContext(std::vector<std::string> &tokens, std::string &locationPath);
void removeSemicolonFromToken(std::vector<std::string> &token);

#endif
