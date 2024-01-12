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

typedef utils::Result<std::vector<Tokenize>, std::string> tokenizeResult;
typedef utils::Result<std::string, std::string> stringResult;

stringResult checkBracketsBalance(const std::vector<Tokenize> &tokens);
stringResult removeSemicolonFromToken(Tokenize &token);
tokenizeResult tokenize(const char *configPath);

#endif
