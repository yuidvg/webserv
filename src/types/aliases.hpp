#pragma once
#include "external.hpp"
#include "structs.hpp"

// config
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
typedef Result<const RequestLine, const HttpRequest> ParseRequestLineResult;
typedef Result<const Headers, const HttpRequest> ParseHeaderResult;
typedef Result<const std::string, const HttpRequest> ParseBodyResult;
typedef Result<const HttpRequest, const HttpRequest> ParseRequestResult;
typedef Result<const RequestLine, const unsigned int> GetRequestLineResult;

// autoindex
typedef Result<const std::string, const HttpResponse> DirectoryListHtmlResult;
typedef Result<const std::string, const std::string> GetIndexFilePathResult;
typedef Result<const Server, const HttpResponse> MatchedServerResult;

// socket
typedef unsigned int Sd;
typedef std::vector<const Sd> Sds;
typedef Result<const Sd, const std::string> NewListenSdResult;
typedef Result<const Sds, const std::string> GetListenSdsResult;
typedef Result<const Sds, const std::string> ReadableSdsResult;

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
typedef Result<const std::map<int, std::string>, const std::string> RedirectResult;

typedef Result<const int, const std::string> StringToIntResult;

} // namespace parseConfig

typedef Result<const bool, const HttpResponse> IsDirectoryResult;
typedef Result<std::string, HttpResponse> FileContentResult;
