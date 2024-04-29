#pragma once
#include "top.hpp"

DirectoryListHtmlResult directoryListHtml(const std::string &path);
HttpRequestsAndEventDatas parseHttpRequests(const EventDatas &httpRequestEventDatas);
HttpRequest getHttpRequest(const int cgiSd);
bool findCgiExtension(const std::string &target, const std::string &extension);
Uri segment(const HttpRequest &);
SocketResult createCgiProcess(const StringMap &envs, const std::string &scriptPath);
ParseCgiResponseResult parseCgiResponse(std::string const &response, const int cgiSd);
HttpMessage processCgiResponse(const CgiResponse &cgiResponse);
CgiRequestOrHttpResponse processHttpRequest(const HttpRequest &);

std::string stringify(const HttpResponse &);

// methods
HttpResponse conductGet(const HttpRequest &httpRequest, const std::string &target);
HttpResponse conductDelete(const HttpRequest &);
HttpResponse conductPost(const HttpRequest &);

HttpResponse getRedirectHttpResponse(const HttpRequest &, const std::string &redirectUrl);

Option<const Socket> findSocket(const int sd, const Sockets &sockets);
std::pair<EventDatas, Strings> retrieveDatas(const KernelEvents &readEvents);
Option<Event> toEvent(const struct kevent &kernelEvent, const Sockets &sockets);
