#pragma once
#include "../webserv.hpp"

ConnectedInternetSocketResult createCgi(const CgiRequest &request, const HttpRequest &httpRequest);
bool findCgiExtension(const std::string &target, const std::string &extension);
Uri segmentUri(const std::string &target, const std::string &cgiExtension);
