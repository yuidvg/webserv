#ifndef PARSE_REQUEST_HPP
#define PARSE_REQUEST_HPP

#include "../webserv.hpp"

typedef Result<RequestLine, int> GetRequestLineResult;

ParseRequestLineResult parseHttpRequestLine(std::istream &httpRequest);
ParseHeaderResult parseHttpHeaders(std::istream &httpRequest);
ParseBodyResult parseHttpBody(std::istream &httpRequest, const Headers &headers,
                              const Server &server);

#endif
