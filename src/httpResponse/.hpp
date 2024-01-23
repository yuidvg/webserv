#pragma once
#include "../webserv.hpp"

HttpResponse response(const ParseRequestResult &requestResult, const Sd &sd, const Servers &servers);
std::string responseText(const HttpResponse &response);

// methods
HttpResponse conductGet(const HttpRequest &request, const Location &location);
HttpResponse conductPost(const HttpRequest &request, const Location &location);
HttpResponse conductDelete(const HttpRequest &request, const Location &location);

// HttpResponse createHttpErrorPage(const int statusCode, const HttpRequest &request, const Server &server);
