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
typedef Result<const std::string, const StatusCode> DirectoryListHtmlResult;
typedef Result<const std::string, const std::string> GetIndexFilePathResult;
typedef Result<const Server, const HttpResponse> MatchedServerResult;

typedef Result<const unsigned int, const HttpResponse> PortNumberResult;

typedef std::pair<int, HttpResponse> ErrorPage;
typedef std::map<int, HttpResponse> ErrorPages;
namespace parseConfig
{
typedef Result<const Server, const std::string> ServerResult;
typedef Result<const std::string, const std::string> NameResult;
typedef Result<const int, const std::string> PortResult;
typedef Result<const Location, const std::string> LocationResult;
typedef Result<const size_t, const std::string> ClientMaxBodySizeResult;
typedef Result<const ErrorPage, const std::string> ErrorPageResult;
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
typedef Result<CgiResponse, std::string> ParseCgiResponseResult;

typedef Result<const std::string, const std::string> ReadFileResult;

typedef Result<const ConnectedInternetSocket, const HttpResponse> ConnectedInternetSocketResult;

typedef std::set<const Socket> Sockets;
typedef Result<const Sockets, const std::string> SocketsResult;

typedef Either<CgiRequest, HttpResponse> CgiRequestOrHttpResponse;

typedef std::queue<const HttpRequest> HttpRequests;

typedef std::vector<const ConnectedInternetSocket> ConnectedInternetSockets;

typedef std::vector<const ConnectedUnixSocket> ConnectedUnixSockets;

typedef Result<const ConnectedUnixSocket, const std::string> ConnectedUnixSocketResult;

typedef Either<ConnectedInternetSocket, ConnectedUnixSocket> ConnectedSocket;

typedef Result<const ConnectedSocket, const std::string> ConnectedSocketResult;

typedef std::vector<SocketBuffer> SocketBuffers;

typedef Result<const HttpResponse, const std::string> HttpResponseResult;

typedef Either<HttpRequest, HttpResponse> HttpMessage;

typedef std::map<const int, const HttpRequest> CgiHttpRequests;

typedef Result<SocketBuffer &, const std::string> FindSocketBufferResult;