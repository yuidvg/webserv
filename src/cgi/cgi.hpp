#ifndef CGI_HPP
#define CGI_HPP

#include "../httpRequest/parsedRequest.hpp"
#include "../utils/utils.hpp"
#include <map>
#include <string>
#include <unistd.h>

namespace cgi
{
typedef Result<std::string, std::string> ResponseResult;
ResponseResult execute(const ParsedRequest request, const Server server, const int clientSocket);
bool shouldUse(const ParsedRequest request, const Server server);
} // namespace cgi

#endif
