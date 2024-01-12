#ifndef PARSEREQUEST_HPP
#define PARSEREQUEST_HPP

#include <cstring>
#include <iostream>
#include <map>
#include <sstream>
#include <unistd.h>
#include <vector>

#include "../config/parseConfig.hpp"
#include "../httpResponse/response.hpp"
#include "../utils/Result.hpp"
#include "../utils/consts.hpp"
#include "../utils/utils.hpp"

#define SUCCESS 0
#define FAILURE 1

#define MAX_LEN 8192

struct ParsedRequest
{
    const std::string method;
    const std::string uri;
    const std::string version;
    const Headers header;
    const std::string body;

    ParsedRequest(const std::string &m, const std::string &u, const std::string &v, const Headers &h,
                  const std::string &b)
        : method(m), uri(u), version(v), header(h), body(b)
    {
    }
};

struct RequestLine
{
    std::string method;
    std::string uri;
    std::string version;
};

typedef Result<ParsedRequest, HttpResponse> ParseRequestResult;
typedef Result<RequestLine, int> ParseRequestLineResult;
typedef Result<Headers, int> ParseHeaderResult;
typedef Result<std::string, int> ParseBodyResult;

ParseRequestLineResult parseHttpRequestLine(std::istream &httpRequest, const Server &server);
ParseHeaderResult parseHttpHeaders(std::istream &httpRequest);
ParseBodyResult parseHttpBody(std::istream &httpRequest, std::map<std::string, std::string> &header);
ParseRequestResult parseHttpRequest(std::istream &httpRequest, const Server &server);

#endif
