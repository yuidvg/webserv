#pragma once
#include "../httpRequestAndConfig/.hpp"
#include "../webserv.hpp"

ParseRequestResult parseHttpRequest(const std::string message, const int port);