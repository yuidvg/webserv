#pragma once
#include "top.hpp"

ConnectedUnixSocketResult createCgiProcess(const StringMap &envs, const std::string &scriptPath);
bool findCgiExtension(const std::string &target, const std::string &extension);
Uri segment(const HttpRequest &);
