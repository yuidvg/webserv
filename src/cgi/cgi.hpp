#ifndef CGI_HPP
#define CGI_HPP

#include "../types/all.hpp"
#include "../utils/all.hpp"

namespace cgi
{
ResponseResult execute(const HttpRequest request, const Server server, const int clientSocket);
bool shouldUse(const HttpRequest request, const Server server);
} // namespace cgi

#endif
