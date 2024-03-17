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
typedef ParseResult<const RequestLine, const HttpResponse> ParseRequestLineResult;
typedef ParseResult<const Headers, const HttpResponse> ParseHeaderResult;
typedef ParseResult<const std::string, const HttpResponse> ParseBodyResult;
typedef ParseResult<const HttpRequest, const HttpResponse> ParseRequestResult;
typedef Result<const RequestLine, const int> GetRequestLineResult;
typedef Result<const std::string, const int> GetHostNameResult;
typedef ParseResult<const std::string, const HttpResponse> UnchunkBodyResult;

// autoindex
typedef Result<const std::string, const HttpResponse> DirectoryListHtmlResult;
typedef Result<const std::string, const std::string> GetIndexFilePathResult;
typedef Result<const Server, const HttpResponse> MatchedServerResult;

// socket
typedef Result<const Client, const std::string> NewClientResult;
typedef Result<const Clients, const std::string> GetListenSocketsResult;
typedef Result<const Clients, const std::string> ReadableWritableSocketsResult;

typedef Result<const unsigned int, const HttpResponse> PortNumberResult;

typedef std::pair<int, HttpResponse> ErrorPage;
typedef std::map<int, HttpResponse> ErrorPages;
namespace parseConfig
{
typedef Result<const Server, const std::string> ServerResult;
typedef Result<const std::string, const std::string> NameResult;
typedef Result<const int, const std::string> PortResult;
typedef Result<const Location, const std::string> LocationResult;
typedef Result<const ErrorPage, const std::string> ErrorPageResult;
typedef Result<const size_t, const std::string> ClientMaxBodySizeResult;
typedef Result<const std::string, const std::string> PathResult;
typedef Result<const std::string, const std::string> RootResult;
typedef Result<const bool, const std::string> AutoindexResult;
typedef Result<const std::string, const std::string> IndexResult;
typedef Result<const std::vector<std::string>, const std::string> AllowMethodsResult;
typedef Result<const std::string, const std::string> CgiExtensionResult;
typedef Result<const std::string, const std::string> UploadPathResult;
typedef Result<const std::string, const std::string> RedirectResult;

} // namespace parseConfig

typedef Result<const int, const std::string> StringToIntResult;
typedef Result<const bool, const int> IsDirectoryResult;
typedef Result<const std::string, const HttpResponse> FileContentResult;

typedef std::vector<std::string> Strings;

typedef Result<const int, const std::string> StoiResult;
typedef Result<CgiResponse, HttpResponse> ParseCgiResponseResult;

typedef Result<const std::string, const std::string> ReadFileResult;

typedef Result<const Socket, const std::string> NewSocketResult;

typedef std::set<const Socket> Sockets;
typedef Result<const Sockets, const std::string> SocketsResult;

typedef Either<CgiRequest, HttpResponse> ImmidiateResponse;

typedef Result<const Cgi, const HttpResponse> CreateCgiResult;