#ifndef PARSEREQUEST_HPP
#define PARSEREQUEST_HPP

#include "../webserv.hpp"

const int MAX_LEN = 8192;

#include "HttpRequest.hpp"

struct RequestLine
{
    std::string method;
    std::string uri;
    std::string version;
};

typedef Result<RequestLine, int> GetRequestLineResult;

typedef Result<RequestLine, HttpResponse> ParseRequestLineResult;
ParseRequestLineResult parseHttpRequestLine(std::istream &httpRequest, const Server &server);
ParseHeaderResult parseHttpHeaders(std::istream &httpRequest);
ParseBodyResult parseHttpBody(std::istream &httpRequest, std::map<std::string, std::string> &headers);
ParseRequestResult parseHttpRequest(std::istream &httpRequest, const Server &server);

#endif
