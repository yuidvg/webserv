#ifndef REQUESTPARSER_HPP
#define REQUESTPARSER_HPP

#include <cstring>
#include <iostream>
#include <map>
#include <sstream>
#include <unistd.h>
#include <vector>

#include "../config/parseConfig.hpp"
#include "../utils/utils.hpp"

#define SUCCESS 0
#define FAILURE 1

#define MAX_LEN 8192

#define OK 200
#define BAD_REQUEST 400
#define METHOD_NOT_ALLOWED 405
#define CONTENT_TOO_LARGE 413
#define REQUEST_URI_TOO_LONG 414
#define NOT_IMPLEMENTED 501
#define Http_VERSION_NOT_SUPPORTED 505

struct ParsedRequest
{
    const std::string method;
    const std::string uri;
    const std::string version;
    const std::map<std::string, std::string> header;
    const std::string body;

    ParsedRequest(const std::string &m, const std::string &u, const std::string &v,
                  const std::map<std::string, std::string> &h, const std::string &b)
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

typedef Result<ParsedRequest, int> HttpParseResult;
typedef Result<RequestLine, int> ParseRequestLineResult;
typedef Result<std::map<std::string, std::string>, int> ParseHeaderResult;
typedef Result<std::string, int> ParseBodyResult;

ParseRequestLineResult parseHttpRequestLine(std::istream &httpRequest, const Server &server);
ParseHeaderResult parseHttpHeaders(std::istream &httpRequest);
ParseBodyResult parseHttpBody(std::istream &httpRequest, std::map<std::string, std::string> &header);
HttpParseResult parseHttpRequest(std::istream &httpRequest, const Server &server);

#endif
