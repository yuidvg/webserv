#ifndef BUILD_RESPONSE_HPP
#define BUILD_RESPONSE_HPP
#include "../webserv.hpp"

HttpResponse response(const ParseRequestResult &requestResult, const Sd &sd, const Servers &servers);
std::string responseText(const HttpResponse &response);

// methods
HttpResponse conductGet(const HttpRequest &request, const Location &location);
HttpResponse conductPost(const HttpRequest &request, const Location &location);
HttpResponse conductDelete(const HttpRequest &request, const Location &location);

HttpResponse createHttpErrorPage(const HttpResponse &httpresponse, const Servers &servers, const Sd &sd);

#endif
