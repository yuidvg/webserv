#pragma once
#include "../httpRequestAndConfig/.hpp"
#include "../types/HttpRequestText.hpp"
#include "../webserv.hpp"

ParseRequestResult parseHttpRequest(HttpRequestText &httpRequestText, const Server &server);
ParseRequestLineResult parseHttpRequestLine(std::istringstream &requestTextStream);
ParseHeaderResult parseHttpHeaders(std::istringstream &requestTextStream);
ParseBodyResult parseHttpBody(std::istringstream &requestTextStream, const Headers &headers, const Server &server);
