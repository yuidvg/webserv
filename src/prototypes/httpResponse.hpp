#pragma once
#include "top.hpp"

std::string stringify(const HttpResponse &);

// methods
HttpResponse conductGet(const HttpRequest &);
HttpResponse conductDelete(const HttpRequest &);
HttpResponse conductPost(const HttpRequest &);

HttpResponse getRedirectResponse(const HttpRequest &, const std::string &redirectUrl);
