#include "HTTPParser.hpp"

static bool	isLineTooLong(const std::string &line)
{
	if (line.length() > MAX_LEN)
		return (true);
	return (false);
}

static bool	checkMethod(std::string method, int &error_code)
{
	std::string	allowed_methods[] = {"GET", "POST", "DELETE"}; // 本来はconfigから取得する？
	for (int i = 0; i < 3; i++)
	{
		if (method == allowed_methods[i])
			break ;
		if (i == 2)
		{
			error_code = HTTP_STATUS_METHOD_NOT_ALLOWED;
			return (false);
		}
	}
	return (true);
}

static bool	checkTarget(std::string uri, int &error_code)
{
	if (uri.find(':') != std::string::npos && uri.find('*') != std::string::npos) // CONNECT, OPTIONSは非対応
	{
		error_code = HTTP_STATUS_BAD_REQUEST;
		return (false);
	}
	return (true);
}

static bool	checkVersion(std::string version, int &error_code)
{
	if (version != "HTTP/1.1")
	{
		error_code = HTTP_STATUS_HTTP_VERSION_NOT_SUPPORTED;
		return (false);
	}
	return (true);
}

ParseRequestLineResult	parseHTTPRequestLine(std::string &httpRequest)
{
	std::string		line;
	std::string		method, uri, version;

	while (customGetLine(httpRequest, line) && line.empty())
		; // 空行を読み飛ばす

	// 有効なリクエストラインがない場合
	if (line.empty())
		return (ParseRequestLineResult::Err(HTTP_STATUS_BAD_REQUEST));
	if (isLineTooLong(line) == true)
		return (ParseRequestLineResult::Err(HTTP_STATUS_REQUEST_URI_TOO_LONG));

	std::istringstream	request_line(line);
	if (!(request_line >> method >> uri >> version) || !request_line.eof()) // メソッドとターゲット、バージョンに分けて格納する
		return (ParseRequestLineResult::Err(HTTP_STATUS_BAD_REQUEST));

	/* エラーチェック */
	int	error_code = HTTP_STATUS_OK;
	if (checkMethod(method, error_code) == false || checkTarget(uri, error_code) == false || checkVersion(version, error_code) == false)
		return (ParseRequestLineResult::Err(error_code));

	RequestLine	request_line_data = {method, uri, version};
	return (ParseRequestLineResult::Ok(request_line_data));
}

ParseHeaderResult	parseHTTPHeaders(std::string &httpRequest)
{
	std::string		line;
	std::map<std::string, std::string>	header;

	while (customGetLine(httpRequest, line))
	{
		if (line.empty())
			break ;
		if (isLineTooLong(line) == true)
			return (ParseHeaderResult::Err(HTTP_STATUS_REQUEST_URI_TOO_LONG));
		if (std::isspace(line[0]))
			return (ParseHeaderResult::Err(HTTP_STATUS_BAD_REQUEST));

		// ヘッダーの解析
		std::string			key, value;
		std::istringstream	header_line(line);

		std::getline(header_line, key, ':');
		std::getline(header_line, value);
		trim(value); // valueの前後の空白を削除する

		if (key.empty() || std::isspace(*(key.end() - 1)) || value.empty())
			return (ParseHeaderResult::Err(HTTP_STATUS_BAD_REQUEST));

		header[toLower(key)] = value; // keyを小文字に変換して格納する
		std::cout << "[key]: " << key << ", [value]: " << header[toLower(key)] << std::endl; // debug
	}
	if (!line.empty() || header.empty())
		return (ParseHeaderResult::Err(HTTP_STATUS_BAD_REQUEST));

	return (ParseHeaderResult::Ok(header));
}

ParseBodyResult	parseChunkedBody(std::string &httpRequest, std::map<std::string, std::string> &header)
{
	std::cout << "====parseChunkedBody====" << std::endl; // debug
	std::string		line;

	if (header["transfer-encoding"] != "chunked")
		return (ParseBodyResult::Err(HTTP_STATUS_NOT_IMPLEMENTED));

	std::string		body;
	while (customGetLine(httpRequest, line))
	{
		if (line.empty())
			break ;
		if (isLineTooLong(line) == true)
			return (ParseBodyResult::Err(HTTP_STATUS_REQUEST_URI_TOO_LONG));

		std::istringstream	chunk_size_line(line);
		int	chunk_size;
		if (!(chunk_size_line >> std::hex >> chunk_size) || !chunk_size_line.eof())
			return (ParseBodyResult::Err(HTTP_STATUS_BAD_REQUEST));

		std::string	chunk;
		if (!customGetLine(httpRequest, chunk) || chunk.length() != static_cast<size_t>(chunk_size))
			return (ParseBodyResult::Err(HTTP_STATUS_BAD_REQUEST));

		body += chunk;
	}
	if (!line.empty())
		return (ParseBodyResult::Err(HTTP_STATUS_BAD_REQUEST));

	return (ParseBodyResult::Ok(body));
}

ParseBodyResult	parsePlainBody(std::string &httpRequest, std::map<std::string, std::string> &header)
{
	std::cout << "====parsePlainBody====" << std::endl; // debug
	std::string		line;

	if (header["content-length"].empty() || !isNumber(header["content-length"]))
		return (ParseBodyResult::Err(HTTP_STATUS_BAD_REQUEST));

	size_t	content_length = std::stoul(header["content-length"]);
	if (content_length > MAX_LEN)
		return (ParseBodyResult::Err(HTTP_STATUS_CONTENT_TOO_LARGE));

	std::string	body;
	if (httpRequest.length() < content_length)
		return (ParseBodyResult::Err(HTTP_STATUS_BAD_REQUEST));

	body = httpRequest.substr(0, content_length);
	httpRequest.erase(0, content_length);
	return (ParseBodyResult::Ok(body));
}

ParseBodyResult	parseHTTPBody(std::string &httpRequest, std::map<std::string, std::string> &header)
{
	std::cout << "====parseBody====" << std::endl; // debug
	std::string		line;

	if (header.find("transfer-encoding") != header.end())
		return (parseChunkedBody(httpRequest, header));
	else if (header.find("content-length") != header.end())
		return (parsePlainBody(httpRequest, header));
	else
		return (ParseBodyResult::Ok(""));
}

ParseResult	parseHTTPRequest(std::string &httpRequest)
{
	ParseRequestLineResult	request_line = parseHTTPRequestLine(httpRequest);
	if (!request_line.ok())
		return (ParseResult::Err(request_line.unwrapErr()));

	ParseHeaderResult		headers = parseHTTPHeaders(httpRequest);
	if (!headers.ok())
		return (ParseResult::Err(headers.unwrapErr()));

	std::map<std::string, std::string>	header = headers.unwrap();
	ParseBodyResult			body = parseHTTPBody(httpRequest, header);
	if (!body.ok())
		return (ParseResult::Err(body.unwrapErr()));

	RequestLine		requestLine = request_line.unwrap();
	ParsedRequest	result(requestLine.method, requestLine.uri, requestLine.version, headers.unwrap(), body.unwrap());
	return (ParseResult::Ok(result));
}
