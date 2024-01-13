#ifndef TOKENIZE_CONFIG_HPP
#define TOKENIZE_CONFIG_HPP

#include "../utils/utils.hpp"
#include "config.hpp"
#include <fstream>
#include <iostream>

struct Token
{
    std::vector<std::string> values;
};

typedef utils::Result<std::vector<Token>, std::string> TokenResult;

TokenResult tokenize(const char *configPath);

#endif
