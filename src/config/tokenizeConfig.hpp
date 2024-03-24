#pragma once

#include "../types/.hpp"

TokensResult tokenize(const char *configPath);
TokensResult extractADirective(std::vector<std::string> &tokens);
