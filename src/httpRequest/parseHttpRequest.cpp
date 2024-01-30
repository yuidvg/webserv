#include ".hpp"

ParseRequestLineResult parseHttpRequestLine(std::istringstream &requestTextStream)
{
    const GetRequestLineResult getRequestLineResult = getRequestLine(requestTextStream);
    if (!getRequestLineResult.success)
        return ParseRequestLineResult::Error(getRequestLineResult.error);

    /* エラーチェック */
    const int statusCode = getRequestLineStatusCode(getRequestLineResult.value);

    return (statusCode != SUCCESS) ? ParseRequestLineResult::Error(statusCode)
                                   : ParseRequestLineResult::Success(getRequestLineResult.value);
}

ParseHeaderResult parseHttpHeaders(std::istringstream &requestTextStream)
{
    std::string line;
    Headers headers;

    while (!(line = getlineCustom(requestTextStream)).empty())
    {
        if (isLineTooLong(line))
            return ParseHeaderResult::Error(BAD_REQUEST);
        if (std::isspace(line[0]))
            return ParseHeaderResult::Error(BAD_REQUEST);

        // ヘッダーの解析
        std::string key, value;
        std::istringstream headerLine(line);

        if (!std::getline(headerLine, key, ':') || !std::getline(headerLine, value))
            return ParseHeaderResult::Error(BAD_REQUEST);

        utils::trim(value);

        if (key.empty() || std::isspace(*(key.end() - 1)) || value.empty())
            return ParseHeaderResult::Error(BAD_REQUEST);

        headers[utils::lowerCase(key)] = value;
    }

    if (!line.empty() || headers.empty())
        return ParseHeaderResult::Error(BAD_REQUEST);

    return ParseHeaderResult::Success(headers);
}

ParseBodyResult parseHttpBody(std::istringstream &requestTextStream, const Headers &headers, const Server &server)
{
    std::string line;
    std::string body((std::istreambuf_iterator<char>(requestTextStream)), std::istreambuf_iterator<char>());

    if (headers.find("transfer-encoding") != headers.end())
    {
        if (headers.at("transfer-encoding") != "chunked" || body.length() > server.clientMaxBodySize)
            return ParseBodyResult::Error(BAD_REQUEST);
    }
    else if (headers.find("content-length") != headers.end())
    {
        if (!utils::isNumber(headers.at("content-length")))
            return ParseBodyResult::Error(BAD_REQUEST);
        const size_t bodySize = static_cast<size_t>(std::stoi(headers.at("content-length")));
        if (bodySize < 0 || bodySize > server.clientMaxBodySize)
            return ParseBodyResult::Error(BAD_REQUEST);
        if (body.length() != server.clientMaxBodySize)
            return ParseBodyResult::Error(BAD_REQUEST);
    }
    else
        return ParseBodyResult::Success(body); // bodyがない場合

    return ParseBodyResult::Success(body);
}

ParseRequestResult parseHttpRequest(HttpRequestText &httpRequestText, const Server &server)
{
    GetTextResult getTextResult = httpRequestText.getText();
    if (!getTextResult.success)
        return ParseRequestResult::Error(HttpResponse(getTextResult.error));

    std::string httpRequest = getTextResult.value;
    std::istringstream requestTextStream(httpRequest);

    const ParseRequestLineResult parseRequestLineResult = parseHttpRequestLine(requestTextStream);
    if (!parseRequestLineResult.success)
        return ParseRequestResult::Error(HttpResponse(parseRequestLineResult.error));

    const ParseHeaderResult headersResult = parseHttpHeaders(requestTextStream);
    if (!headersResult.success)
        return ParseRequestResult::Error(HttpResponse(headersResult.error));

    const Headers headers = headersResult.value;

    const ParseBodyResult body = parseHttpBody(requestTextStream, headers, server);
    if (!body.success)
        return ParseRequestResult::Error(HttpResponse(body.error));

    const RequestLine requestLine = parseRequestLineResult.value;
    const HttpRequest result(requestLine.method, requestLine.target, requestLine.version, headers, body.value,
                             httpRequestText.getHostName().value);
    return ParseRequestResult::Success(result);
}
