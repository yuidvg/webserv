#pragma once

#include "types/bottom.hpp"

const std::string SERVER_PROTOCOL = "HTTP/1.1";
const std::string GATEWAY_INTERFACE = "CGI/1.1";
const std::string SERVER_SOFTWARE = "webserv/1.0";

// domain sockets
const unsigned int SERVER_END = 0;
const unsigned int CGI = 1;

// charsets
const std::string CRLF = "\r\n";

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

const HttpResponse getMethodNotAllowedResponse(const HttpRequest &httpRequest, const std::string &allowedMethods);

extern Config CONFIG;

const std::string BASE_64_CHARS = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

const std::string URI_SCHEME_DELIMITER = "://";

const std::string EMPTY_LINE = "\n\n";

const std::string SERVER_ERROR_BODY =
    " <html> <head> <title>500 Internal Server Error</title> </head> <body> <h1>500 Internal Server "
    "Error</h1> <p>The server encountered an internal error or misconfiguration and was unable to "
    "complete your request.<br /></p> </body> </html>";

const std::string BAD_REQUEST_BODY =
    " <html> <head> <title>400 Bad Request</title> </head> <body> <h1>400 Bad Request</h1> <p>Your "
    "browser sent a request that this server could not understand.<br /></p> </body> </html>";

const std::string CONTENT_TYPE_HTML = "text/html";

extern const int KQ;

extern std::queue<const HttpRequest> HTTP_REQUESTS;

extern ConnectedInternetSockets CLIENT_SOCKETS;

extern ConnectedUnixSockets CGI_SOCKETS;

extern SocketBuffers SOCKET_BUFFERS;

extern CgiHttpRequests CGI_HTTP_REQUESTS;

#define EVENT_BATCH_SIZE 1024
