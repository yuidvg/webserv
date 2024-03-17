#pragma once
#include "../webserv.hpp"

ImmidiateResponse retrieveImmidiateResponse(const HttpRequest &request, const Connection &connection);
std::string responseText(const HttpResponse &response);
// methods
HttpResponse conductGet(const Uri &uri, const Location &location);
HttpResponse conductDelete(const std::string &path);
HttpResponse conductPost(const HttpRequest &request, const Location &location);

HttpResponse provideErrorResponse(const HttpResponse &httpResponse, const Server &server);
HttpResponse redirectResponse(const std::string &redirectUrl);
