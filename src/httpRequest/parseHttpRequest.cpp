#include ".hpp"

ParseRequestLineResult parseHttpRequestLine(std::istringstream &requestTextStream)
{
    const GetRequestLineResult getRequestLineResult = getRequestLine(requestTextStream);
    if (getRequestLineResult.success)
    {
        const int statusCode = getRequestLineStatusCode(getRequestLineResult.value);
        if (statusCode == SUCCESS)
            return ParseRequestLineResult::Success(getRequestLineResult.value);
        else
            return ParseRequestLineResult::Error(statusCode);
    }
    else
        return ParseRequestLineResult::Error(getRequestLineResult.error);
}

ParseHeaderResult parseHttpHeaders(std::istringstream &requestTextStream)
{
    std::string line;
    Headers headers;

    while (!(line = utils::getlineCustom(requestTextStream)).empty())
    {
        if (!isLineTooLong(line) && !isspace(line[0]))
        {
            std::string key, value;
            std::istringstream headerLine(line);

            if (std::getline(headerLine, key, ':') && std::getline(headerLine, value))
            {
                utils::trim(value);
                if (!key.empty() && !std::isspace(*(key.end() - 1)) && !value.empty())
                    headers[utils::lowerCase(key)] = value;
                else
                    return ParseHeaderResult::Error(BAD_REQUEST);
            }
            else
                return ParseHeaderResult::Error(BAD_REQUEST);
        }
        else
            return ParseHeaderResult::Error(BAD_REQUEST);
    }

    if (line.empty() && !headers.empty())
        return ParseHeaderResult::Success(headers);
    else
        return ParseHeaderResult::Error(BAD_REQUEST);
}

ParseBodyResult parseHttpBody(std::istringstream &requestTextStream, const Headers &headers, const Server &server,
                              const std::string &method)
{
    std::string line;
    const std::string body((std::istreambuf_iterator<char>(requestTextStream)), std::istreambuf_iterator<char>());

    if (headers.find("transfer-encoding") != headers.end())
    {
        if (headers.at("transfer-encoding") == "chunked" && body.length() <= server.clientMaxBodySize)
            return ParseBodyResult::Success(body);
        else
            return ParseBodyResult::Error(BAD_REQUEST);
    }
    else if (headers.find("content-length") != headers.end())
    {
        if (utils::isNumber(headers.at("content-length")))
        {
            const size_t bodySize = static_cast<size_t>(std::stoi(headers.at("content-length")));
            if ((0 < bodySize && bodySize <= server.clientMaxBodySize) && body.length() == bodySize)
                return ParseBodyResult::Success(body);
            else
                return ParseBodyResult::Error(BAD_REQUEST);
        }
        else
            return ParseBodyResult::Error(BAD_REQUEST);
    }
    else
    {
        if (method == "GET" || method == "DELETE")
            return ParseBodyResult::Success("");
        else if (method == "POST" && body.empty())
            return ParseBodyResult::Error(BAD_REQUEST);
        else
            return ParseBodyResult::Success(body);
    }
}

ParseRequestResult parseHttpRequest(HttpRequestText &httpRequestText, const Server &server)
{
    GetTextResult getTextResult = httpRequestText.getText();
    if (getTextResult.success)
    {
        const std::string httpRequest = getTextResult.value;
        std::istringstream requestTextStream(httpRequest);

        const ParseRequestLineResult parseRequestLineResult = parseHttpRequestLine(requestTextStream);
        if (parseRequestLineResult.success)
        {
            const ParseHeaderResult headersResult = parseHttpHeaders(requestTextStream);
            if (headersResult.success)
            {
                const Headers headers = headersResult.value;
                const ParseBodyResult body =
                    parseHttpBody(requestTextStream, headers, server, parseRequestLineResult.value.method);
                if (body.success)
                {
                    const RequestLine requestLine = parseRequestLineResult.value;
                    const HttpRequest result(requestLine.method, requestLine.target, headers, body.value,
                                             httpRequestText.getHostName().value);
                    return ParseRequestResult::Success(result);
                }
                else
                    return ParseRequestResult::Error(HttpResponse(body.error));
            }
            else
                return ParseRequestResult::Error(HttpResponse(headersResult.error));
        }
        else
            return ParseRequestResult::Error(HttpResponse(parseRequestLineResult.error));
    }
    else
        return ParseRequestResult::Error(HttpResponse(getTextResult.error));
}
