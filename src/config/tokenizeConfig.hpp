#pragma once

#include "../types/bottom.hpp"

TokensResult tokenize(const char *configPath);
TokensResult extractADirective(std::vector<std::string> &tokens);
