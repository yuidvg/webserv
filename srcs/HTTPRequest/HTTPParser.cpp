#include "HTTPParser.hpp"

static bool	isLineTooLong(const std::string &line)
{
	if (line.length() > MAX_LEN)
		return (true);
	return (false);
}

static bool	checkRequestLine(std::string &method, std::string &uri, std::string &version, int &error_code, const Server &server)
{
	if (uri.find(':') != std::string::npos && uri.find('*') != std::string::npos) // CONNECT, OPTIONSは非対応
	{
		error_code = BAD_REQUEST;
		return (false);
	}

	std::vector<std::string>	allowed_methods;
	std::vector<Location>::const_iterator it;
	for (it = server.locations.begin(); it != server.locations.end(); ++it)
	{
		if (!it->front_path.empty() && uri.find(it->front_path) != std::string::npos)
		{
			if (uri.find(it->front_path) != std::string::npos)
			{
				allowed_methods = it->allow_method;
				break; // マッチしたらループを抜ける
			}
			if (uri.find(it->front_path) == std::string::npos)
			{
				error_code = BAD_REQUEST;
				return (false);
			}
		}
		// else if (!it.back_path.empty() && uri.rfind(it.back_path) != std::string::npos)
		// {
		// 	// マッチしなかった場合は、後方一致のパスをチェックする
		// }
		else
		{
			error_code = BAD_REQUEST;
			return (false);
		}
	}

	for (int i = 0; i < 3; i++)
	{
		std::cout << "method: " << method << ", allowed_methods[i]: " << allowed_methods[i] << std::endl; // debug
		if (method == allowed_methods[i])
			break ;
		if (i == 2)
		{
			error_code = METHOD_NOT_ALLOWED;
			return (false);
		}
	}
	if (version != "HTTP/1.1")
	{
		error_code = HTTP_VERSION_NOT_SUPPORTED;
		return (false);
	}
	return (true);
}

ParseRequestLineResult	parseHTTPRequestLine(std::istream &httpRequest, const Server &server)
{
	std::string		line;
	std::string		method, uri, version;

	while (std::getline(httpRequest, line) && line.empty())
		; // 空行を読み飛ばす

	// 有効なリクエストラインがない場合
	if (line.empty())
		return (ParseRequestLineResult::Err(BAD_REQUEST));
	if (isLineTooLong(line) == true)
		return (ParseRequestLineResult::Err(REQUEST_URI_TOO_LONG));

	std::istringstream	request_line(line);
	if (!(request_line >> method >> uri >> version) || !request_line.eof()) // メソッドとターゲット、バージョンに分けて格納する
		return (ParseRequestLineResult::Err(BAD_REQUEST));

	/* エラーチェック */
	int	error_code = OK;
	if (!checkRequestLine(method, uri, version, error_code, server))
		return (ParseRequestLineResult::Err(error_code));

	RequestLine	request_line_data = {method, uri, version};
	return (ParseRequestLineResult::Ok(request_line_data));
}

ParseHeaderResult	parseHTTPHeaders(std::istream &httpRequest)
{
	std::string		line;
	std::map<std::string, std::string>	header;

	while (std::getline(httpRequest, line))
	{
		if (line.empty())
			break ;
		if (isLineTooLong(line) == true)
			return (ParseHeaderResult::Err(REQUEST_URI_TOO_LONG));
		if (std::isspace(line[0]))
			return (ParseHeaderResult::Err(BAD_REQUEST));

		// ヘッダーの解析
		std::string			key, value;
		std::istringstream	header_line(line);

		std::getline(header_line, key, ':');
		std::getline(header_line, value);
		trim(value); // valueの前後の空白を削除する

		if (key.empty() || std::isspace(*(key.end() - 1)) || value.empty())
			return (ParseHeaderResult::Err(BAD_REQUEST));

		header[toLower(key)] = value; // keyを小文字に変換して格納する
		std::cout << "[key]: " << key << ", [value]: " << header[toLower(key)] << std::endl; // debug
	}
	if (!line.empty() || header.empty())
		return (ParseHeaderResult::Err(BAD_REQUEST));

	return (ParseHeaderResult::Ok(header));
}

ParseBodyResult	parseChunkedBody(std::istream &httpRequest, std::map<std::string, std::string> &header)
{
	std::cout << "====parseChunkedBody====" << std::endl; // debug
	std::string		line;

	if (header["transfer-encoding"] != "chunked")
		return (ParseBodyResult::Err(NOT_IMPLEMENTED));

	std::string		body;
	while (std::getline(httpRequest, line))
	{
		// if (line.empty())
		// 	break ;
		if (isLineTooLong(line) == true)
			return (ParseBodyResult::Err(REQUEST_URI_TOO_LONG));

		std::istringstream	chunk_size_line(line);
		int	chunk_size;
		if (!(chunk_size_line >> std::hex >> chunk_size) || !chunk_size_line.eof())
			return (ParseBodyResult::Err(BAD_REQUEST));

		// チャンクのサイズを読み取り、0でない限り続ける
		if (chunk_size == 0)
			break ;

		char*	buffer = new char[chunk_size];
		httpRequest.read(buffer, chunk_size);
		body.append(buffer, chunk_size);
		delete[] buffer;

		// チャンクの末尾のCRLFを読み飛ばす
		std::getline(httpRequest, line);
	}
	std::cout << "Processed body: " << body << std::endl;

	return (ParseBodyResult::Ok(body));
}

ParseBodyResult	parsePlainBody(std::istream &httpRequest, std::map<std::string, std::string> &header)
{
	std::cout << "====parsePlainBody====" << std::endl; // debug
	std::string		line;

	if (header["content-length"].empty() || !isNumber(header["content-length"]))
		return (ParseBodyResult::Err(BAD_REQUEST));

	size_t	content_length = std::stoul(header["content-length"]);
	if (content_length > MAX_LEN)
		return (ParseBodyResult::Err(CONTENT_TOO_LARGE));

	std::string	body(content_length, '\0');
	if (httpRequest.read(&body[0], content_length))
		return (ParseBodyResult::Err(BAD_REQUEST));

	return (ParseBodyResult::Ok(body));
}

ParseBodyResult	parseHTTPBody(std::istream &httpRequest, std::map<std::string, std::string> &header)
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

HTTPParseResult	parseHTTPRequest(std::istream &httpRequest, const Server &server)
{
	ParseRequestLineResult	request_line = parseHTTPRequestLine(httpRequest, server);
	if (!request_line.ok())
		return (HTTPParseResult::Err(request_line.unwrapErr()));

	ParseHeaderResult		headers = parseHTTPHeaders(httpRequest);
	if (!headers.ok())
		return (HTTPParseResult::Err(headers.unwrapErr()));

	std::map<std::string, std::string>	header = headers.unwrap();
	ParseBodyResult			body = parseHTTPBody(httpRequest, header);
	if (!body.ok())
		return (HTTPParseResult::Err(body.unwrapErr()));

	RequestLine		requestLine = request_line.unwrap();
	ParsedRequest	result(requestLine.method, requestLine.uri, requestLine.version, headers.unwrap(), body.unwrap());
	return (HTTPParseResult::Ok(result));
}
