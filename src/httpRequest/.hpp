#pragma once
#include "../webserv.hpp"
#include "../types/HttpRequestText.hpp"
#include "../httpRequestAndConfig/.hpp"

ParseRequestResult parseHttpRequest(HttpRequestText &httpRequestText);
ParseRequestLineResult parseHttpRequestLine(std::istringstream &requestTextStream);
ParseHeaderResult parseHttpHeaders(std::istringstream &requestTextStream);
ParseBodyResult parseHttpBody(std::istringstream &requestTextStream, const Headers &headers);
