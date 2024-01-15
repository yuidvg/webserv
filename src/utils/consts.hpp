#ifndef CONSTS_HPP
#define CONSTS_HPP

#include <string>

const std::string SERVER_PROTOCOL = "HTTP/1.1";
const std::string GATEWAY_INTERFACE = "CGI/1.1";
const std::string SERVER_SOFTWARE = "webserv/1.0";

// pipes
const unsigned int IN = 1;
const unsigned int OUT = 0;

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

// tokens
const std::string SERVER = "server";
const std::string SERVER_NAME = "server_name";
const std::string LISTEN = "listen";
const std::string LOCATION = "location";
const std::string ALLOW_METHOD = "allow_method";
const std::string CGI_EXTENSION = "cgi_extension";
const std::string UPLOAD_PATH = "upload_path";
const std::string RETURN = "return";
const std::string ROOT = "root";
const std::string ERROR_PAGE = "error_page";
const std::string CLIENT_MAX_BODY_SIZE = "client_max_body_size";
const std::string INDEX = "index";
const std::string AUTOINDEX = "autoindex";
const std::string OPEN_BRACKET = "{";
const std::string CLOSE_BRACKET = "}";

#endif
