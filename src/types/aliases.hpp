#pragma once
#include "external.hpp"
#include "structs.hpp"
enum StatusCode
{
    SUCCESS = 200,
    // 300番は複数のリダイレクト先が存在し、クライアントが選択する必要がある
    REDIRECTION = 301,
    BAD_REQUEST = 400,
    SERVER_ERROR = 500
};

// CONFIG
typedef Result<const std::vector<std::string>, const std::string> TokensResult;
typedef Result<const Servers, const std::string> ConfigResult;
typedef Result<const Server, const std::string> ParseServerResult;
typedef Result<const Location, const std::string> ParseLocationResult;
typedef Result<const std::string, const std::string> StringResult;

typedef std::vector<const EventData> EventDatas;

// http
namespace parseHttpRequest
{
struct FirstBlockResult
{
    const std::string method;
    const std::string target;
    const std::string version;
    const Headers headers;
};
typedef ParseResult<const FirstBlockResult, const EventBlock> ParseFirstBlockResult;
typedef Either<const EventData, const HttpRequest> EventDataOrParsedRequest;
typedef ParseResult<const std::string, const int> ParseBodyResult;
typedef Result<const std::string, const int> GetHostNameResult;
typedef ParseResult<const std::string, const int> UnchunkBodyResult;
typedef Result<const HttpRequest, const std::string> HttpRequestResult;
typedef std::queue<const EventDataOrParsedRequest> ParseHttpRequestResults;
} // namespace parseHttpRequest

// autoindex
typedef Result<const std::string, const StatusCode> DirectoryListHtmlResult;
typedef Result<const std::string, const std::string> GetIndexFilePathResult;
typedef Result<const Server, const HttpResponse> MatchedServerResult;

typedef Result<const unsigned int, const HttpResponse> PortNumberResult;

typedef std::map<int, std::string> ErrorPagePaths;
namespace parseConfig
{
typedef Result<const Server, const std::string> ServerResult;
typedef Result<const std::string, const std::string> NameResult;
typedef Result<const int, const std::string> PortResult;
typedef Result<const Location, const std::string> LocationResult;
typedef Result<const size_t, const std::string> ClientMaxBodySizeResult;
typedef Result<const ErrorPagePaths, const std::string> ErrorPagePathsResult;
typedef Result<const std::string, const std::string> PathResult;
typedef Result<const std::string, const std::string> RootResult;
typedef Result<const bool, const std::string> AutoindexResult;
typedef Result<const std::string, const std::string> IndexResult;
typedef Result<const std::vector<std::string>, const std::string> AllowMethodsResult;
typedef Result<const std::string, const std::string> CgiExtensionResult;
typedef Result<const std::string, const std::string> UploadPathResult;
typedef Result<const std::string, const std::string> RedirectResult;

} // namespace parseConfig

typedef Result<const Socket, const std::string> SocketResult;
typedef Result<const int, const std::string> StringToIntResult;
typedef Result<const bool, HttpResponse> IsDirectoryResult;
typedef Result<const std::string, const StatusCode> FileContentResult;

typedef std::vector<std::string> Strings;

typedef Result<const int, const std::string> StoiResult;
typedef Result<const CgiResponse, const std::string> ParseCgiResponseResult;

typedef Result<const std::string, const std::string> ReadFileResult;


typedef std::vector<const Socket> Sockets;
typedef Result<const Sockets, const std::string> SocketsResult;

typedef Result<const HttpResponse, const std::string> HttpResponseResult;

typedef Either<HttpRequest, HttpResponse> HttpMessage;

typedef std::map<const int, const HttpRequest> CgiHttpRequests;
typedef std::queue<const CgiResponse> CgiResponses;

typedef Either<CgiRequest, HttpResponse> CgiRequestOrHttpResponse;

typedef std::map<const std::string, const std::string> StringMap;

typedef std::vector<const struct kevent> KEvents;
