#pragma once

#include "../globals.hpp"
#include "../types/bottom.hpp"

namespace parseConfig
{
ConfigResult parseConfig(const char *configPath);
ParseServerResult parseServerContext(std::vector< std::string > &tokens);
ParseLocationResult parseLocationContext(std::vector< std::string > &tokens, std::string &locationPath);
} // namespace parseConfig
