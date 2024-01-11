#ifndef PARSEREQUEST_HPP
#define PARSEREQUEST_HPP

#include <cstring>
#include <iostream>
#include <map>
#include <sstream>
#include <unistd.h>
#include <vector>

#include "../config/parseConfig.hpp"
#include "../httpResponse/HttpResponse.hpp"
#include "../utils/utils.hpp"

const int MAX_LEN = 8192;

#include "HttpRequest.hpp"

struct RequestLine
{
    std::string method;
    std::string uri;
    std::string version;
};

typedef Result<HttpRequest, HttpResponse> ParseRequestResult;
ParseRequestLineResult parseHttpRequestLine(std::istream &httpRequest, const Server &server);

typedef Result<Headers, HttpResponse> ParseHeaderResult;
ParseHeaderResult parseHttpHeaders(std::istream &httpRequest);

typedef Result<std::string, HttpResponse> ParseBodyResult;
ParseBodyResult parseHttpBody(std::istream &httpRequest, std::map<std::string, std::string> &headers);

typedef Result<RequestLine, HttpResponse> ParseRequestLineResult;
ParseRequestResult parseHttpRequest(std::istream &httpRequest, const Server &server);

#endif
