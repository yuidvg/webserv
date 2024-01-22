#ifndef PARSE_REQUEST_HPP
#define PARSE_REQUEST_HPP

#include "../webserv.hpp"

typedef Result<RequestLine, int> GetRequestLineResult;

ParseRequestLineResult parseHttpRequestLine(std::istream &httpRequest);
ParseHeaderResult parseHttpHeaders(std::istream &httpRequest);
ParseBodyResult parseHttpBody(std::istream &httpRequest, std::map<std::string, std::string> &headers);
ParseRequestResult parseHttpRequest(std::istream &httpRequest);

#endif
