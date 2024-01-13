#ifndef BUILD_RESPONSE_HPP
#define BUILD_RESPONSE_HPP
#include <string>
#include "../config/config.hpp"
#include "../httpRequest/parseRequest.hpp"
#include "HttpResponse.hpp"

HttpResponse response(const ParseRequestResult requestResult, const Server server);
std::string makeResponseMessage(const HttpResponse response);

#endif