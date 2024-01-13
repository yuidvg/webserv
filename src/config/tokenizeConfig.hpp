#ifndef TOKENIZE_CONFIG_HPP
#define TOKENIZE_CONFIG_HPP

#include "../utils/utils.hpp"
#include "config.hpp"
#include <fstream>
#include <iostream>

struct Tokenize
{
    std::string key;
    std::vector<std::string> values;
};

typedef utils::Result<std::vector<Tokenize>, std::string> TokenizeResult;

TokenizeResult tokenize(const char *configPath);

#endif
