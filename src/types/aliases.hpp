#pragma once
#include "external.hpp"
#include "structs.hpp"

// CONFIG
typedef Result<const std::vector<std::string>, const std::string> TokensResult;
typedef Result<const Servers, const std::string> ConfigResult;
typedef Result<const Server, const std::string> ParseServerResult;
typedef Result<const Location, const std::string> ParseLocationResult;
typedef Result<const std::string, const std::string> StringResult;

// http
struct RequestLine
{
    const std::string method;
    const std::string target;
    const std::string version;
};
typedef Result<const RequestLine, const HttpResponse> ParseRequestLineResult;
typedef Result<const Headers, const HttpResponse> ParseHeaderResult;
typedef Result<const std::string, const HttpResponse> ParseBodyResult;
typedef Result<const HttpRequest, const HttpResponse> ParseRequestResult;
typedef Result<const RequestLine, const int> GetRequestLineResult;

// autoindex
typedef Result<const std::string, const HttpResponse> DirectoryListHtmlResult;
typedef Result<const std::string, const std::string> GetIndexFilePathResult;
typedef Result<const Server, const HttpResponse> MatchedServerResult;

// socket
typedef Result<const Socket, const std::string> NewListenSocketResult;
typedef Result<const Sockets, const std::string> GetListenSocketsResult;
typedef Result<const Sockets, const std::string> ReadableSocketsResult;

typedef Result<const unsigned int, const HttpResponse> PortNumberResult;

typedef Result<const HttpResponse, const HttpResponse> ResponseResult;

namespace parseConfig
{
typedef Result<const Server, const std::string> ServerResult;
typedef Result<const std::string, const std::string> NameResult;
typedef Result<const int, const std::string> PortResult;
typedef Result<const Location, const std::string> LocationResult;
typedef Result<const std::map<int, std::string>, const std::string> ErrorPagesResult;
typedef Result<const size_t, const std::string> ClientMaxBodySizeResult;
typedef Result<const std::string, const std::string> PathResult;
typedef Result<const std::string, const std::string> RootResult;
typedef Result<const bool, const std::string> AutoindexResult;
typedef Result<const std::string, const std::string> IndexResult;
typedef Result<const std::vector<std::string>, const std::string> AllowMethodsResult;
typedef Result<const std::string, const std::string> CgiExtensionResult;
typedef Result<const std::string, const std::string> UploadPathResult;
typedef Result<const std::string, const std::string> RedirectResult;

typedef Result<const int, const std::string> StringToIntResult;

} // namespace parseConfig

typedef Result<const bool, const HttpResponse> IsDirectoryResult;
typedef Result<const std::string, const HttpResponse> FileContentResult;

typedef std::vector<std::string> Strings;

typedef Result<CgiResponse, HttpResponse> ParseCgiResponseResult;
