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

// charsets
const std::string CRLF = "\r\n";

// status code classes
enum STATUS_CODE_CLASSES
{
    SUCCESS = 200,
    REDIRECTION = 300,
    BAD_REQUEST = 400,
    SERVER_ERROR = 500
};

#endif
