#pragma once
#include "../webserv.hpp"

ImmidiateResponse retrieveImmidiateResponse(const HttpRequest &request, const Connection &connection);
std::string responseText(const HttpResponse &response);
// methods
HttpResponse conductGet(const Uri &uri, const Location &location, const Server &server);
HttpResponse conductDelete(const std::string &path, const Server &server);
HttpResponse conductPost(const HttpRequest &request, const Location &location, const Server &server);

HttpResponse redirectResponse(const std::string &redirectUrl);