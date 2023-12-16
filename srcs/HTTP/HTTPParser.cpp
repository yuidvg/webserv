#include "HTTPParser.hpp"

HTTPParser::HTTPParser(std::string &request)
{
	executeParse(request);
}

HTTPParser::HTTPParser(const HTTPParser &src)
{
	(*this) = src;
}

HTTPParser	&HTTPParser::operator=(const HTTPParser &rhs)
{
	if (this != &rhs)
	{
		// copy
	}
	return (*this);
}

HTTPParser::~HTTPParser(void) {}

std::string	HTTPParser::getMethod(void) const
{
	return (_method);
}

std::string	HTTPParser::getTarget(void) const
{
	return (_url);
}

std::string	HTTPParser::getVersion(void) const
{
	return (_version);
}

static std::string	getRequestLine(std::string &data)
{
	std::string		request_line;
	size_t			pos;

	pos = data.find("\n");
	if (pos == std::string::npos)
	{
		std::cout << "here" << std::endl; // debug
		return (request_line);
	}
	request_line = data.substr(0, pos);
	data.erase(0, pos + 1);
	return (request_line);
}

// static std::string	getHeader(std::string &data)
// {
// 	std::string		line;
// 	size_t			pos;

// 	std::istringstream	iss(data);
// 	while (std::getline(iss, line))
// 	{
// 		if (strlen(line.c_str()) == 0)
// 			break ;
// 		/* _headerに格納していく */
		
// 		/* dataからlineを削除していく */
// 	}
// 	return (header);
// }

void	HTTPParser::executeParse(std::string &data)
{
	std::string		request_line;

	std::cout << "[data]\n" << data << std::endl; // debug
	request_line = getRequestLine(data);
	std::cout << "request_line: " << request_line << std::endl; // debug
	/* メソッドとターゲット、バージョンを格納する(error処理も) */
	std::istringstream	iss(request_line);
	iss >> _method >> _url >> _version;


	/* ヘッダーを格納する(error処理も) */
	std::cout << "[data]\n" << data << std::endl; // debug


	/* ボディを格納する(error処理も) */
	std::cout << "[data] \n" << data << std::endl; // debug
}
