#pragma once
#include "../webserv.hpp"

CgiRequest retrieveCgiRequest(const HttpRequest &request, const Client &socket, const Uri &uri);
CreateCgiResult createCgi(const CgiRequest &request, const ErrorPages &errorPages, const Client &client,
                          const HttpRequest &httpRequest);
bool findCgiExtension(const std::string &target, const std::string &extension);
Uri segmentUri(const std::string &target, const std::string &cgiExtension);
