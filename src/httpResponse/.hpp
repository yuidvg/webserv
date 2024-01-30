#pragma once
#include "../webserv.hpp"

HttpResponse response(const ParseRequestResult &requestResult, const Server &server);
std::string responseText(const HttpResponse &response);

// methods
HttpResponse conductGet(const HttpRequest &request, const Location &location);
HttpResponse conductPost(const HttpRequest &request, const Location &location);
HttpResponse conductDelete(const HttpRequest &request, const Location &location);

HttpResponse provideErrorResponse(const HttpResponse &httpResponse, const Server &server);
HttpResponse redirectResponse(const std::string &redirectUrl);
