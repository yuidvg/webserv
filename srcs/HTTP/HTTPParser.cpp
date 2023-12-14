#include "HTTPParser.hpp"

HTTPPARSER::HTTPPARSER(char *request)
{
	parseHTTPRequest(request);
}

HTTPPARSER::HTTPPARSER(const HTTPPARSER &src) : _requestline(src._requestline), _header(src._header), _body(src._body) {}

HTTPPARSER	&HTTPPARSER::operator=(const HTTPPARSER &src)
{
	if (this != &src)
	{
		// copy
	}
	return (*this);
}

HTTPPARSER::~HTTPPARSER(void) {}

char	*HTTPPARSER::getRequestline(void) const
{
	if (_requestline == NULL)
		throw std::runtime_error("Requestline is NULL");
	return (_requestline);
}

// char	*HTTPPARSER::getMethod(void) const
// {
// 	if (_method == NULL)
// 		throw std::runtime_error("Method is NULL");
// 	return (_method);
// }

char	*HTTPPARSER::getHeader(void) const
{
	if (_header == NULL)
		throw std::runtime_error("Header is NULL");
	return (_header);
}

char	*HTTPPARSER::getBody(void) const
{
	if (_body == NULL)
		throw std::runtime_error("Body is NULL");
	return (_body);
}

void	HTTPPARSER::parseHTTPRequest(char *request)
{
	_requestline = strtok(request, "\r\n");
	std::istringstream	tmp(_requestline);
	_header = strtok(NULL, "\r\n");
	_body = strtok(NULL, "\r\n");
}
