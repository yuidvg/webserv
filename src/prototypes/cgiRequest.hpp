#pragma once
#include "top.hpp"

ConnectedUnixSocketResult createCgiProcess(const CgiRequest &request);
bool findCgiExtension(const std::string &target, const std::string &extension);
Uri segmentize(const HttpRequest &);
