#pragma once
#include "../webserv.hpp"

ParseCgiResponseResult parseCgiResponse(std::string const &response);
HttpResponse processCgiResponse(const CgiResponse &cgiResponse, const HttpRequest &request, const Connection &socket);
