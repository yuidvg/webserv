#pragma once
#include "../webserv.hpp"

ResponseResult execute(const HttpRequest request, const Server server, const int clientSocket);
bool shouldUse(const HttpRequest request, const Server server);
