#pragma once
#include "top.hpp"

ParseRequestResult parseHttpRequest(const std::string &request, const int port);
HttpRequest getHttpRequest(const int cgiSd);