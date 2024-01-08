#ifndef CONSTS_HPP
#define CONSTS_HPP

#include <string>

const std::string SERVER_PROTOCOL = "HTTP/1.1";
const std::string GATEWAY_INTERFACE = "CGI/1.1";
const std::string SERVER_SOFTWARE = "webserv/1.0";

// pipes
const unsigned int IN = 1;
const unsigned int OUT = 0;

// colors
const std::string RED = "\x1b[31m";
const std::string GREEN = "\x1b[32m";
const std::string YELLO = "\x1b[33m";
const std::string NORMAL = "\x1b[0m";

#endif
