#ifndef PARSECONFIG_HPP
#define PARSECONFIG_HPP

#include "../webserv.hpp"

namespace parseConfig
{
ConfigResult parseConfig(const char *configPath);
ParseServerResult parseServerContext(std::vector<std::string> &tokens);
ParseLocationResult parseLocationContext(std::vector<std::string> &tokens, std::string &locationPath);
}

#endif
