#include "HTTPParser.hpp"

bool	isLineTooLong(const std::string &line)
{
	if (line.length() > MAX_LEN)
	{
		_error_code = HTTP_STATUS_REQUEST_URI_TOO_LONG; // 414
		return (true);
	}
	return (false);
}

bool	checkMethod(void)
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

bool	checkTarget(void)
{
	if (_url.find(':') != std::string::npos && _url.find('*') != std::string::npos) // CONNECT, OPTIONSは非対応
	{
		_error_code = HTTP_STATUS_BAD_REQUEST; // 400
		return (false);
	}
	return (true);
}

bool	checkVersion(void)
{
	if (_version != "HTTP/1.1")
	{
		_error_code = 505;
		return (false);
	}
	return (true);
}

ParseResult	parseHTTPRequestLine(std::string &data)
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

ParseResult	parseHTTPHeader(std::string &data)
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
		trim(value); // valueの前後の空白を削除する

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

ParseResult	parseHTTPBody(std::string &data)
{
	std::string			line;
	std::istringstream	iss(data);

	std::cout << "====parseBody====" << std::endl; // debug

	if (data.empty())
		std::cout << "dataなし!!" << std::endl; // debug
	else
		std::cout << "data: " << data << std::endl; // debug

	if (_header["Content-Length"].empty() || _header["Transfer-Encoding"].empty())
	{
		_error_code = HTTP_STATUS_BAD_REQUEST; // 400
		return (FAILURE);
	}

	if (!_header["Content-Length"].empty() && (stoi(_header["Content-Length"]) > MAX_LEN))
	{
		_error_code = 413;
		return (FAILURE);
	}

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
	}
	return (SUCCESS);
}

void	executeParse(std::string &data)
{
	std::string		request_line;

	_error_code = 200;
	if (parseHTTPRequestLine(data) == FAILURE)
		return;

	if (parseHTTPHeader(data) == FAILURE)
		return;

	if (parseHTTPBody(data) == FAILURE)
		return;
}
