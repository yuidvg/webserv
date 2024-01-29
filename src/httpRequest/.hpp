#pragma once
#include "../webserv.hpp"
#include "../types/HttpRequestText.hpp"
#include "../httpRequestAndConfig/.hpp"

ParseRequestResult parseHttpRequest(HttpRequestText &httpRequestText);
ParseRequestLineResult parseHttpRequestLine(HttpRequestText &httpRequestText);
ParseHeaderResult parseHttpHeaders(HttpRequestText &httpRequestText);
ParseBodyResult parseHttpBody(HttpRequestText &httpRequestText, const Headers &headers);
