#ifndef HTTPRESPONSE_HPP
# define HTTPRESPONSE_HPP

# include <cstring>
# include <iostream>

# define HTTP_RESPONSE_200 "HTTP/1.1 200 OK\r\n"
# define HTTP_RESPONSE_201 "HTTP/1.1 201 Created\r\n"
# define HTTP_RESPONSE_202 "HTTP/1.1 202 Accepted\r\n"
# define HTTP_RESPONSE_204 "HTTP/1.1 204 No Content\r\n"
# define HTTP_RESPONSE_301 "HTTP/1.1 301 Moved Permanently\r\n"
# define HTTP_RESPONSE_302 "HTTP/1.1 302 Found\r\n"
# define HTTP_RESPONSE_304 "HTTP/1.1 304 Not Modified\r\n"
# define HTTP_RESPONSE_400 "HTTP/1.1 400 Bad Request\r\n"
# define HTTP_RESPONSE_401 "HTTP/1.1 401 Unauthorized\r\n"
# define HTTP_RESPONSE_403 "HTTP/1.1 403 Forbidden\r\n"
# define HTTP_RESPONSE_404 "HTTP/1.1 404 Not Found\r\n"
# define HTTP_RESPONSE_405 "HTTP/1.1 405 Method Not Allowed\r\n"
# define HTTP_RESPONSE_406 "HTTP/1.1 406 Not Acceptable\r\n"
# define HTTP_RESPONSE_407 "HTTP/1.1 407 Proxy Authentication Required\r\n"
# define HTTP_RESPONSE_408 "HTTP/1.1 408 Request Timeout\r\n"
# define HTTP_RESPONSE_409 "HTTP/1.1 409 Conflict\r\n"
# define HTTP_RESPONSE_410 "HTTP/1.1 410 Gone\r\n"
# define HTTP_RESPONSE_411 "HTTP/1.1 411 Length Required\r\n"
# define HTTP_RESPONSE_412 "HTTP/1.1 412 Precondition Failed\r\n"
# define HTTP_RESPONSE_413 "HTTP/1.1 413 Payload Too Large\r\n"
# define HTTP_RESPONSE_414 "HTTP/1.1 414 URI Too Long\r\n"
# define HTTP_RESPONSE_415 "HTTP/1.1 415 Unsupported Media Type\r\n"
# define HTTP_RESPONSE_416 "HTTP/1.1 416 Range Not Satisfiable\r\n"


class HTTPResponse
{
	public:
		;
};

#endif
