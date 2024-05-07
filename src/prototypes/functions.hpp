#pragma once
#include "top.hpp"

#include "../functions/functors.hpp"
#include "../functions/templateFunctions.tpp"

DirectoryListHtmlResult directoryListHtml(const std::string &path);
std::pair<HttpRequests, EventDatas> parseHttpRequests(const EventDatas &httpRequestEventDatas);
HttpRequest getHttpRequest(const int cgiSd);
bool findCgiExtension(const std::string &target, const std::string &extension);
Uri segment(const HttpRequest &);
Option<Socket> createCgiProcess(const StringMap &envs, const std::string &scriptPath);
CgiResponses parseCgiResponses(const EventDatas &cgiResponseEventDatas);
HttpMessage processCgiResponse(const CgiResponse &cgiResponse);
HttpResponseOrCgiRequestOrEventData processHttpRequest(const HttpRequest &httpRequest);

std::string stringify(const HttpResponse &);

// methods
HttpResponse conductGet(const HttpRequest &httpRequest, const std::string &target);
HttpResponse conductDelete(const HttpRequest &);
HttpResponseOrEventData conductPost(const HttpRequest &);

HttpResponse getRedirectHttpResponse(const HttpRequest &, const std::string &redirectUrl);

Option<const Socket> findSocket(const int sd, const Sockets &sockets);
EventDatas retrieveDatas(const Events &clientReadEvents);
Events toEvents(const KernelEvents &kernelEvents, const Sockets &sockets);
EventDatas unifyData(EventDatas eventDatas);
// process
HttpResponses_CgiRequests_EventDatas processHttpRequests(const HttpRequests &httpRequests);
std::pair<const HttpResponses, const HttpRequests> processCgiResponses(const CgiResponses &cgiResponses);
Option<EventData> findEventData(const int fd, const EventDatas &eventDatas);
