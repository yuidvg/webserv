#include ".hpp"

/* ========  helper functions ======== */
std::vector<std::string> removeEmptyBlocks(const std::vector<std::string> &blocks)
{
    std::vector<std::string>::const_iterator it = blocks.begin();
    while (it != blocks.end() && it->empty())
    {
        ++it;
    }
    std::vector<std::string> nonEmptyBlocks(it, blocks.end());
    return nonEmptyBlocks;
}

bool isLineTooLong(const std::string &line)
{
    if (line.length() > MAX_LEN)
        return true;
    else
        return false;
}

int getRequestLineStatusCode(const RequestLine requestLine)
{
    if (requestLine.target.find(':') != std::string::npos &&
        requestLine.target.find('*') != std::string::npos) // CONNECT, OPTIONSは非対応
        return BAD_REQUEST;

    if (requestLine.version != SERVER_PROTOCOL)
        return BAD_REQUEST;

    return SUCCESS;
}

bool validateAndDecodeTarget(std::string &target)
{
    std::string decodedTarget;
    std::string::iterator it = target.begin();

    while (it != target.end())
    {
        if (*it == '%')
        {
            if (it + 2 >= target.end() || !std::isxdigit(*(it + 1)) || !std::isxdigit(*(it + 2)))
                return false;
            decodedTarget += utils::hexToUtf8Char(target.substr(it - target.begin() + 1, 2));
            it += 3;
        }
        else
        {
            decodedTarget += *it;
            it++;
        }
    }
    target = decodedTarget;
    return true;
}

GetRequestLineResult getRequestLine(std::istringstream &requestTextStream)
{
    std::string method, target, version;
    std::string line;

    while ((line = utils::getlineCustom(requestTextStream)).empty())
        ;

    if (!requestTextStream.eof() && !isLineTooLong(line))
    {
        std::istringstream requestLine(line);
        if ((requestLine >> method >> target >> version) && requestLine.eof() && validateAndDecodeTarget(target))
        {
            const RequestLine requestLineElements = {method, target, version};
            return GetRequestLineResult::Success(requestLineElements);
        }
        else
            return GetRequestLineResult::Error(BAD_REQUEST);
    }
    else
        return GetRequestLineResult::Error(BAD_REQUEST);
}

/* ======== main funcitons ======== */
ParseRequestLineResult parseHttpRequestLine(std::istringstream &requestTextStream)
{
    // 最初に空行がある場合は読み飛ばす
    std::string firstLine = utils::getlineCustom(requestTextStream);
    while (firstLine.empty() && !requestTextStream.eof())
        firstLine = utils::getlineCustom(requestTextStream);
    if (!firstLine.empty())
        requestTextStream.seekg(0);

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
    std::string headerLine;
    Headers headers;

    while (!(headerLine = utils::getlineCustom(requestTextStream)).empty())
    {
        if (!isLineTooLong(headerLine) && !isspace(headerLine[0]))
        {
            std::string key, value;
            std::istringstream headerLineStream(headerLine);

            if (std::getline(headerLineStream, key, ':') && std::getline(headerLineStream, value))
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

    if (headerLine.empty() && !headers.empty())
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

ParseRequestResult parseHttpRequest(const std::string message)
{
    const std::vector<std::string> blocks = utils::split(message, CRLF + CRLF);
    const std::vector<std::string> nonEmptyBlocks = removeEmptyBlocks(blocks);
    if (nonEmptyBlocks.size() < 2)
        return ParseRequestResult::Pending();
    else
    {
        std::istringstream requestTextStream(nonEmptyBlocks[0]);
        const ParseRequestLineResult parseRequestLineResult = parseHttpRequestLine(requestTextStream);
        if (parseRequestLineResult.status == SUCCESS)
        {
            const ParseHeaderResult parseHeaderResult = parseHttpHeaders(requestTextStream);
            if (parseHeaderResult.status == SUCCESS)
            {
                const ParseBodyResult parseBodyResult = parseHttpBody(
                    requestTextStream, parseHeaderResult.value, CONFIG.getServer(parseRequestLineResult.value.host, 0),
                    parseRequestLineResult.value.method);
                if (parseBodyResult.status == SUCCESS)
                {
                    return ParseRequestResult::Success(
                        HttpRequest(parseRequestLineResult.value, parseHeaderResult.value, parseBodyResult.value, parseRequestLineResult.value.host));
                }
                else
                    return ParseRequestResult::Error(parseBodyResult.error);
            }
            else
                return ParseRequestResult::Error(parseHeaderResult.error);
        }
        else
            return ParseRequestResult::Error(parseRequestLineResult.error);
    }
}
