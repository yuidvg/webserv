#pragma once

#include "types/bottom.hpp"

FindSocketBufferResult findSocketBuffer(const int sd);
bool setOutbound(int sd, std::string content);
