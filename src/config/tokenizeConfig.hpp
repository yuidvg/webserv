#ifndef TOKENIZE_CONFIG_HPP
#define TOKENIZE_CONFIG_HPP

#include "../types/all.hpp"
#include "../utils/all.hpp"

TokensResult tokenize(const char *configPath);
TokensResult extractADirective(std::vector<std::string> &tokens);

#endif
