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

typedef std::vector<EventData> EventDatas;

// http
struct RequestLine
{
    const std::string method;
    const std::string target;
    const std::string version;
    RequestLine() : method(""), target(""), version(""){};
    RequestLine(const std::string method, const std::string target, const std::string version)
        : method(method), target(target), version(version){};
};
struct FirstBlock
{
    const RequestLine requestLine;
    const Headers headers;
    FirstBlock() : requestLine(RequestLine("", "", "")), headers(Headers()){};
    FirstBlock(const RequestLine requestLine, const Headers headers) : requestLine(requestLine), headers(headers){};
};
typedef Result<const RequestLine, const std::string> SplitRequestLineResult;
typedef Result<const RequestLine, const std::string> ParseRequestLineResult;
typedef ParseResult<const Headers, const EventData> ParseHeaderResult;
typedef ParseResult<const std::string, const std::string> ParseBodyResult;
typedef ParseResult<const std::string, const std::string> UnchunkBodyResult;
typedef ParseResult<const FirstBlock, const std::string> ParseFirstBlockResult;
typedef Either<const EventData, const HttpRequest> EventDataOrParsedRequest;
typedef Either<const HttpResponse, const EventData> HttpResponseOrEventData;

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

typedef std::set<const Socket> Sockets;
typedef Result<const Sockets, const std::string> SocketsResult;

typedef Result<const HttpResponse, const std::string> HttpResponseResult;

typedef Either<HttpRequest, HttpResponse> HttpMessage;

typedef std::map<const int, const HttpRequest> CgiHttpRequests;
typedef std::vector<const CgiResponse> CgiResponses;

typedef std::vector<const HttpRequest> HttpRequests;

typedef Ternary<HttpResponse, CgiRequest, EventData> HttpResponseOrCgiRequestOrEventData;

typedef std::map<const std::string, const std::string> StringMap;

typedef std::vector<const struct kevent> KernelEvents;

typedef std::pair<const Socket, const std::string> SocketError;

typedef std::vector<const SocketError> SocketErrors;

typedef std::vector<const Event> Events;

typedef std::vector<const HttpResponse> HttpResponses;
typedef std::vector<const CgiRequest> CgiRequests;

// clang-format off
typedef std::pair<const HttpResponses, std::pair<const CgiRequests, const EventDatas> > HttpResponses_CgiRequests_EventDatas;
// clang-format on
