#pragma once
#include "top.hpp"

ConnectedInternetSocketResult createCgiProcess(const CgiRequest &, const HttpRequest &);
bool findCgiExtension(const std::string &target, const std::string &extension);
Uri segmentize(const HttpRequest &);
