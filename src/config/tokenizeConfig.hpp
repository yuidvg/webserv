#ifndef TOKENIZE_CONFIG_HPP
#define TOKENIZE_CONFIG_HPP

#include "../utils/utils.hpp"
#include "config.hpp"
#include <fstream>
#include <iostream>

typedef utils::Result<std::vector<std::string>, std::string> TokensResult;

TokensResult tokenize(const char *configPath);
TokensResult extractADirective(std::vector<std::string> &tokens);

#endif
