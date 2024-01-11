#ifndef TOKENIZE_CONFIG_HPP
#define TOKENIZE_CONFIG_HPP

#include "../webserv.hpp"

TokensResult tokenize(const char *configPath);
TokensResult extractADirective(std::vector<std::string> &tokens);

#endif
