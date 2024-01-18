#ifndef ALIASES_HPP
#define ALIASES_HPP

#include "external.hpp"
#include "structs.hpp"

// config
typedef Result<std::vector<std::string>, std::string> TokensResult;
typedef Result<const Servers, const std::string> ConfigResult;
typedef Result<const Server, const std::string> ParseServerResult;
typedef Result<const Location, const std::string> ParseLocationResult;
typedef Result<const std::string, const std::string> StringResult;

// http
struct RequestLine
{
    std::string method;
    std::string uri;
    std::string version;
};
typedef Result<RequestLine, HttpResponse> ParseRequestLineResult;
typedef Result<Headers, HttpResponse> ParseHeaderResult;
typedef Result<std::string, HttpResponse> ParseBodyResult;
typedef Result<HttpRequest, HttpResponse> ParseRequestResult;

// socket
typedef Result<const Socket, const std::string> NewSocketResult;
typedef std::vector<const Socket> Sockets;

namespace cgi
{
typedef Result<std::string, std::string> ResponseResult;
} // namespace cgi


#endif
