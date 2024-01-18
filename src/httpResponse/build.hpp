#ifndef BUILD_RESPONSE_HPP
#define BUILD_RESPONSE_HPP
#include "../webserv.hpp"

HttpResponse response(const ParseRequestResult requestResult, const Server server);
std::string makeResponseMessage(const HttpResponse response);

#endif