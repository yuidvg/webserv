#pragma once
#include "../types/.hpp"
#include "../utils/.hpp"

namespace cgi
{
ResponseResult execute(const HttpRequest request, const Server server, const int clientSocket);
bool shouldUse(const HttpRequest request, const Server server);
} // namespace cgi
