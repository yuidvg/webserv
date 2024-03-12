#pragma once

#include "types/.hpp"

const std::string SERVER_PROTOCOL = "HTTP/1.1";
const std::string GATEWAY_INTERFACE = "CGI/1.1";
const std::string SERVER_SOFTWARE = "webserv/1.0";

// pipes
const unsigned int IN = 1;
const unsigned int OUT = 0;

// charsets
const std::string CRLF = "\r\n";

// status code classes
enum StatusCodeClasses
{
    SUCCESS = 200,
    // 300番は複数のリダイレクト先が存在し、クライアントが選択する必要がある
    REDIRECTION = 301,
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

// httpRequest
const size_t MAX_LEN = 8192;

// httpResponse
const HttpResponse BAD_REQUEST_RESPONSE =
    HttpResponse(BAD_REQUEST,
                 " <html> <head> <title>400 Bad Request</title> </head> <body> <h1>400 Bad Request</h1> <p>Your "
                 "browser sent a request that this server could not understand.<br /></p> </body> </html>",
                 "text/html");

const HttpResponse SERVER_ERROR_RESPONSE =
    HttpResponse(SERVER_ERROR,
                 " <html> <head> <title>500 Internal Server Error</title> </head> <body> <h1>500 Internal Server "
                 "Error</h1> <p>The server encountered an internal error or misconfiguration and was unable to "
                 "complete your request.<br /></p> </body> </html>",
                 "text/html");

const HttpResponse SUCCESS_RESPONSE = HttpResponse(SUCCESS);

const HttpResponse METHOD_NOT_ALLOWED_RESPONSE(const std::string &allowedMethods);

extern Config CONFIG;

const std::string BASE_64_CHARS = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

const std::string URI_SCHEME_DELIMITER = "://";

const std::string EMPTY_LINE = "\n\n";

extern const int KQ;

extern Sockets SOCKETS;

#define EVENT_BATCH_SIZE 1024
