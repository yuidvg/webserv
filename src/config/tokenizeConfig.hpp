#pragma once

#include "../webserv.hpp"

TokensResult tokenize(const char *configPath);
TokensResult extractADirective(std::vector<std::string> &tokens);


