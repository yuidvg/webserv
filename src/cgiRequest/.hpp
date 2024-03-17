#pragma once
#include "../webserv.hpp"

HttpResponse executeCgi(const HttpRequest &request, const Connection &socket, const Uri &scriptUri);
bool findCgiExtension(const std::string &target, const std::string &extension);
CgiRequest retrieveCgiRequest(const HttpRequest &request, const Connection &socket, const Uri &uri);
Uri segmentUri(const std::string &target, const std::string &cgiExtension);