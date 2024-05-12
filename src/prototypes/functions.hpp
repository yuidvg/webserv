#pragma once
#include "top.hpp"

#include "templateFunctions.tpp"

DirectoryListHtmlResult directoryListHtml(const std::string &path);
std::pair< HttpRequests, EventDatas > parseHttpRequests(const EventDatas &httpRequestEventDatas);
Uri segment(const HttpRequest &);
Option< Socket > createCgiProcess(const HttpRequest &httpRequest, const std::string &scriptPath);
CgiResponses parseCgiResponses(const EventDatas &cgiResponseEventDatas);
HttpMessage processCgiResponse(const CgiResponse &cgiResponse);
std::pair< Option< HttpResponse >, Option< EventData > > processHttpRequest(const HttpRequest &httpRequest);

std::string stringify(const HttpResponse &);

// methods
HttpResponse conductGet(const HttpRequest &httpRequest, const std::string &target);
HttpResponse conductDelete(const HttpRequest &);
// clang-format off
std::pair<HttpResponse, Option<EventData> > conductPost(const HttpRequest &);
// clang-format on
HttpResponse getRedirectHttpResponse(const HttpRequest &, const std::string &redirectUrl);

Option< const Socket > findSocket(const int sd);
EventDatas retrieveDatas(const Events &clientReadEvents);
Events toEvents(const KernelEvents &kernelEvents);
EventDatas unifyData(EventDatas eventDatas);
// process
std::pair< HttpResponses, EventDatas > processHttpRequests(const HttpRequests &httpRequests);
std::pair< const HttpResponses, const HttpRequests > processCgiResponses(const CgiResponses &cgiResponses);
Option< EventData > findEventData(const int fd, const EventDatas &eventDatas);
