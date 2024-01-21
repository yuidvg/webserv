#ifndef PARSE_REQUEST_HPP
#define PARSE_REQUEST_HPP

#include "../webserv.hpp"

ParseRequestResult parseHttpRequest(std::istream &httpRequest, const Servers &servers, const Socket &socket);
ParseRequestLineResult parseHttpRequestLine(std::istream &httpRequest);
ParseHeaderResult parseHttpHeaders(std::istream &httpRequest);
ParseBodyResult parseHttpBody(std::istream &httpRequest, const Headers &headers,
                              const Server &server);

#endif
