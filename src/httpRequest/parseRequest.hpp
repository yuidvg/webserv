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
typedef Result<RequestLine, HttpResponse> ParseRequestLineResult;
typedef Result<Headers, HttpResponse> ParseHeaderResult;
typedef Result<std::string, HttpResponse> ParseBodyResult;

ParseRequestLineResult parseHttpRequestLine(std::istream &httpRequest, const Server &server);
ParseHeaderResult parseHttpHeaders(std::istream &httpRequest);
ParseBodyResult parseHttpBody(std::istream &httpRequest, std::map<std::string, std::string> &headers);
ParseRequestResult parseHttpRequest(std::istream &httpRequest, const Server &server);

#endif
