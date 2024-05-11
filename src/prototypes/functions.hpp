#pragma once
#include "top.hpp"

#include "templateFunctions.tpp"

DirectoryListHtmlResult directoryListHtml(const std::string &path);
std::pair<HttpRequests, EventDatas> parseHttpRequests(const EventDatas &httpRequestEventDatas);
HttpRequest getHttpRequest(const int cgiSd);
bool findCgiExtension(const std::string &target, const std::string &extension);
Uri segment(const HttpRequest &);
Option<Socket> createCgiProcess(const HttpRequest &httpRequest, const std::string &scriptPath);
CgiResponses parseCgiResponses(const EventDatas &cgiResponseEventDatas);
HttpMessage processCgiResponse(const CgiResponse &cgiResponse);
Option<HttpResponse> processHttpRequest(const HttpRequest &httpRequest);

std::string stringify(const HttpResponse &);

// methods
HttpResponse conductGet(const HttpRequest &httpRequest, const std::string &target);
HttpResponse conductDelete(const HttpRequest &);
HttpResponse conductPost(const HttpRequest &);

HttpResponse getRedirectHttpResponse(const HttpRequest &, const std::string &redirectUrl);

Option<const Socket> findSocket(const int sd);
EventDatas retrieveDatas(const Events &clientReadEvents);
Events toEvents(const KernelEvents &kernelEvents);
EventDatas unifyData(EventDatas eventDatas);
// process
HttpResponses processHttpRequests(const HttpRequests &httpRequests);
std::pair<const HttpResponses, const HttpRequests> processCgiResponses(const CgiResponses &cgiResponses);
Option<EventData> findEventData(const int fd, const EventDatas &eventDatas);
