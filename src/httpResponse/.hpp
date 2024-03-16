#pragma once
#include "../webserv.hpp"

HttpResponse response(const ParseRequestResult &requestResult, const Connection &socket);
std::string responseText(const HttpResponse &response);
HttpResponse responseToValidRequest(const HttpRequest &request, const Connection &socket);
// methods
HttpResponse conductGet(const Uri &uri, const Location &location);
HttpResponse conductDelete(const std::string &path);
HttpResponse conductPost(const HttpRequest &request, const Location &location);

HttpResponse provideErrorResponse(const HttpResponse &httpResponse, const Server &server);
HttpResponse redirectResponse(const std::string &redirectUrl);
