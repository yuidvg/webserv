#ifndef BUILD_RESPONSE_HPP
#define BUILD_RESPONSE_HPP
#include "../webserv.hpp"

HttpResponse response(const ParseRequestResult requestResult, const Sd sd);
std::string responseText(const HttpResponse response);

#endif
