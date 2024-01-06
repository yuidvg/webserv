#ifndef HTTPPARSER_HPP
# define HTTPPARSER_HPP

# include <cstring>
# include <unistd.h>
# include <iostream>
# include <sstream>
# include <map>
# include <vector>

# include "utils.hpp"
# include "../Result/Result.hpp"

# define SUCCESS 0
# define FAILURE 1

# define MAX_LEN 8192

# define HTTP_STATUS_OK 200
# define HTTP_STATUS_BAD_REQUEST 400
# define HTTP_STATUS_METHOD_NOT_ALLOWED 405
# define HTTP_STATUS_CONTENT_TOO_LARGE 413
# define HTTP_STATUS_REQUEST_URI_TOO_LONG 414
# define HTTP_STATUS_NOT_IMPLEMENTED 501
# define HTTP_STATUS_HTTP_VERSION_NOT_SUPPORTED 505

struct ParsedRequest
{
	const std::string							method;
	const std::string							uri;
	const std::string							version;
	const std::map<std::string, std::string>	header;
	const std::string							body;

	ParsedRequest(const std::string &m, const std::string &u, const std::string &v, const std::map<std::string, std::string> &h, const std::string &b) 
			: method(m), uri(u), version(v), header(h), body(b) {}
};

struct RequestLine
{
	std::string	method;
	std::string	uri;
	std::string	version;
};

typedef Result<ParsedRequest, int>						HTTPParseResult;
typedef Result<RequestLine, int>						ParseRequestLineResult;
typedef Result<std::map<std::string, std::string>, int>	ParseHeaderResult;
typedef Result<std::string, int>						ParseBodyResult;

ParseRequestLineResult	parseHTTPRequestLine(std::string &httpRequest, std::vector<std::string> &allowed_methods);
ParseHeaderResult		parseHTTPHeaders(std::string &httpRequest);
ParseBodyResult			parseHTTPBody(std::string &httpRequest, std::map<std::string, std::string> &header);
HTTPParseResult			parseHTTPRequest(std::string &httpRequest, std::vector<std::string> &allowed_methods);

#endif
