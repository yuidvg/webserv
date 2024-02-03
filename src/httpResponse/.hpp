#pragma once
#include "../webserv.hpp"

HttpResponse response(const ParseRequestResult &requestResult, const Socket &socket);
std::string responseText(const HttpResponse &response);

// methods
HttpResponse conductGet(const Uri &uri, const Location &location);
HttpResponse conductPost(const HttpRequest &request, const Uri &uri);
HttpResponse conductDelete(const Uri &uri);

HttpResponse provideErrorResponse(const HttpResponse &httpResponse, const Server &server);
HttpResponse redirectResponse(const std::string &redirectUrl);
