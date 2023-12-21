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

int			HTTPParser::getErrorCode(void) const
{
	return (_error_code);
}

bool	HTTPParser::isLineTooLong(const std::string &line)
{
	if (line.length() > MAX_LEN)
	{
		_error_code = HTTP_STATUS_REQUEST_URI_TOO_LONG; // 414
		return (true);
	}
	return (false);
}

bool	HTTPParser::checkMethod(void)
{
	std::string	allowed_methods[] = {"GET", "POST", "DELETE"}; // 本来はconfigから取得する？
	for (int i = 0; i < 3; i++)
	{
		if (_method == allowed_methods[i])
			break ;
		if (i == 2)
		{
			_error_code = 405;
			return (false);
		}
	}
	return (true);
}

bool	HTTPParser::checkTarget(void)
{
	if (_url.find(':') != std::string::npos) // connectは対応しない
	{
		_error_code = HTTP_STATUS_BAD_REQUEST; // 400
		return (false);
	}
	return (true);
}

bool	HTTPParser::checkVersion(void)
{
	if (_version != "HTTP/1.1")
	{
		_error_code = 505;
		return (false);
	}
	return (true);
}

int	HTTPParser::parseHTTPRequestLine(std::string &data)
{
	std::string			request_line;

	std::cout << "====parseRequestLine====" << std::endl; // debug

	while (customGetLine(data, request_line) && request_line.empty())
		; // 空行を読み飛ばす

	// 有効なリクエストラインがない場合
	if (request_line.empty())
	{
		_error_code = HTTP_STATUS_BAD_REQUEST; // 400
		return (FAILURE);
	}
	if (isLineTooLong(request_line) == true)
		return (FAILURE);

	std::cout << "request_line: " << request_line << std::endl; // debug

	std::istringstream	line(request_line);
	if (!(line >> _method >> _url >> _version) || !line.eof()) // メソッドとターゲット、バージョンに分けて格納する
	{
		_error_code = HTTP_STATUS_BAD_REQUEST; // 400
		return (FAILURE);
	}

	/* エラーチェック */
	if (checkMethod() == false || checkTarget() == false || checkVersion() == false)
		return (FAILURE);

	return (SUCCESS);
}

int	HTTPParser::parseHTTPHeader(std::string &data)
{
	std::string			line;

	std::cout << "====parseHeader====" << std::endl; // debug

	while (customGetLine(data, line))
	{
		if (line.empty())
			break ;
		if (isLineTooLong(line) == true)
			return (FAILURE);
		if (std::isspace(line[0]))
		{
			_error_code = HTTP_STATUS_BAD_REQUEST; // 400
			return (FAILURE);
		}

		// ヘッダーの解析
		std::string			key, value;
		std::istringstream	iss(line);

		std::getline(iss, key, ':');
		std::getline(iss, value);
		if (std::isspace(value[0]))
			value.erase(0, 1); // 先頭の空白を削除

		if (key.empty() || std::isspace(*(key.end() - 1)) || value.empty())
		{
			_error_code = HTTP_STATUS_BAD_REQUEST; // 400
			return (FAILURE);
		}

		_header[key] = value;
		std::cout << "[key]: " << key << ", [value]: " << _header[key] << std::endl; // debug
	}
	if (!line.empty() || _header.empty())
	{
		_error_code = HTTP_STATUS_BAD_REQUEST; // 400
		return (FAILURE);
	}
	return (SUCCESS);
}

// int	HTTPParser::parseHTTPBody(std::string &data)
// {
// 	std::string			line;

// 	std::cout << "====parseBody====" << std::endl; // debug

// 	if (_header.find("Content-Length") == _header.end() || _header.find("Transfer-Encoding") != _header.end())
// 	{
// 		std::cout << "bodyなし" << std::endl; // debug
// 		return (SUCCESS);
// 	}
// 	std::cout << "test: " << _header["Content-Length"] << std::endl; // debug
// 	while (true)
// 	{
// 		customGetLine(data, line);
// 		if (line.empty())
// 			break ;
// 		unsigned long	content_length = std::stoul(_header["Content-Length"]); // try-catchでエラー処理を追加する必要あり?
// 		if (line.length() != content_length)
// 		{
// 			_error_code = 414;
// 			return (FAILURE);
// 		}
// 		if (line[0] == ' ')
// 		{
// 			_error_code = HTTP_STATUS_BAD_REQUEST; // 400
// 			return (FAILURE);
// 		}

// 		/* _bodyに格納する */
// 		_body += line + "\n";
// 	}
// 	return (SUCCESS);
// }

void	HTTPParser::executeParse(std::string &data)
{
	std::string		request_line;

	_error_code = 200;
	if (parseHTTPRequestLine(data) == FAILURE)
		return;

	if (parseHTTPHeader(data) == FAILURE)
		return;

	// if (parseHTTPBody(data) == FAILURE)
	// 	return;
}
