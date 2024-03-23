#pragma once
#include "../webserv.hpp"

CgiRequestOrHttpResponse getCgiRequestOrHttpResponse(const HttpRequest &request, const Client &client,
                                                     const ErrorPages &errorPages);
std::string responseText(const HttpResponse &response);
// methods
HttpResponse conductGet(const Uri &uri, const Location &location, const ErrorPages &errorPages);
HttpResponse conductDelete(const std::string &path, const ErrorPages &errorPages);
HttpResponse conductPost(const HttpRequest &request, const Location &location, const ErrorPages &errorPages);

HttpResponse redirectResponse(const std::string &redirectUrl);
