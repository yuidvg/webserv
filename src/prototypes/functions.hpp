#pragma once
#include "top.hpp"

DirectoryListHtmlResult directoryListHtml(const std::string &path);
ParseRequestResult parseHttpRequest(const std::string &request, const int port);
HttpRequest getHttpRequest(const int cgiSd);
bool findCgiExtension(const std::string &target, const std::string &extension);
Uri segment(const HttpRequest &);
ConnectedUnixSocketResult createCgiProcess(const StringMap &envs, const std::string &scriptPath);
ParseCgiResponseResult parseCgiResponse(std::string const &response, const int cgiSd);
HttpMessage processCgiResponse(const CgiResponse &cgiResponse);
CgiRequestOrHttpResponse processHttpRequest(const HttpRequest &);

std::string stringify(const HttpResponse &);

// methods
HttpResponse conductGet(const HttpRequest &);
HttpResponse conductDelete(const HttpRequest &);
HttpResponse conductPost(const HttpRequest &);

HttpResponse getRedirectHttpResponse(const HttpRequest &, const std::string &redirectUrl);

SocketBuffer &getSocketBuffer(const int sd);

std::string resolvePath(const std::string &path, const Location &location);
std::string resolvePath(const std::string &path, const std::string &location);
ParsedHttpRequests parseHttpRequests(SocketBuffer &socketBuffer, const ConnectedInternetSocket &socket);
