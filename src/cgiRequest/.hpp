#pragma once
#include "../webserv.hpp"

HttpResponse executeCgi(const HttpRequest &request, const Socket &socket, const Uri &scriptUri);
bool findCgiExtension(const std::string &target, const std::string &extension);

Uri segmentUri(const std::string &target, const std::string &cgiExtension);