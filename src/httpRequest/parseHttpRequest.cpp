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

GetHostNameResult getHostName(const Headers &headers)
{
    if (headers.find("host") != headers.end())
        return GetHostNameResult::Success(headers.at("host"));
    else
        return GetHostNameResult::Error(BAD_REQUEST);
}

UnchunkBodyResult unchunkBody(const std::string &body, const size_t maxBodySize)
{
    std::string unchunkedBody = "";
    std::istringstream bodyStream(body);
    std::string line;

    while (!(line = utils::getlineCustom(bodyStream)).empty())
    {
        std::istringstream chunkSizeLine(line);
        int chunkSize;
        if (!(chunkSizeLine >> std::hex >> chunkSize) || !chunkSizeLine.eof())
            return UnchunkBodyResult::Error(BAD_REQUEST);
        if (chunkSize == 0)
            break;

        if (unchunkedBody.length() + chunkSize > maxBodySize)
            return UnchunkBodyResult::Error(BAD_REQUEST);

        char buffer[chunkSize];
        bodyStream.read(buffer, chunkSize);
        unchunkedBody.append(buffer, chunkSize);

        // チャンクの末尾のCRLFを読み飛ばす
        std::getline(bodyStream, line);
    }
    return UnchunkBodyResult::Success(unchunkedBody);
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
    // tellgで現在の位置を取得、その先からstringを取得する
    const std::streampos currentPos = requestTextStream.tellg();
    std::string headerText = requestTextStream.str().substr(currentPos);

    std::vector<std::string> header = utils::split(headerText, CRLF);
    if (header.size() == 0)
        return ParseHeaderResult::Pending();
    else
    {
        Headers headers;
        for (std::vector<std::string>::iterator it = header.begin(); it != header.end(); ++it)
        {
            std::string::size_type pos = it->find(":");
            if (pos != std::string::npos)
            {
                std::string key = it->substr(0, pos);
                std::string value = it->substr(pos + 1);
                std::transform(key.begin(), key.end(), key.begin(), ::tolower); // headerのkeyは大文字小文字を区別しない
                value = utils::trim(value); // valueの前後の空白を削除
                headers[key] = value;
            }
            else
                return ParseHeaderResult::Error(BAD_REQUEST);
        }
        return ParseHeaderResult::Success(headers);
    }
}

ParseBodyResult parseHttpBody(const std::string &body, const Headers &headers, const Server &server,
                              const std::string &method)
{
    std::string line;

    if (headers.find("transfer-encoding") != headers.end())
    {
        if (headers.at("transfer-encoding") == "chunked" && body.length() <= server.clientMaxBodySize)
        {
            const UnchunkBodyResult unchunkedBodyResult = unchunkBody(body, server.clientMaxBodySize);
            if (unchunkedBodyResult.success)
                return ParseBodyResult::Success(unchunkedBodyResult.value);
            else
                return ParseBodyResult::Error(unchunkedBodyResult.error);
        }
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

ParseRequestResult parseHttpRequest(const Socket &socket)
{
    const std::string message = socket.getReceivedMessage();
    const std::vector<std::string> blocks = utils::split(message, CRLF + CRLF);
    const std::vector<std::string> nonEmptyBlocks = removeEmptyBlocks(blocks);
    if (nonEmptyBlocks.size() < 2 && message.find("POST") != std::string::npos)
        return ParseRequestResult::Pending();
    else
    {
        if (nonEmptyBlocks.size() == 0)
            return ParseRequestResult::Error(BAD_REQUEST);
        std::istringstream requestTextStream(nonEmptyBlocks[0]);
        const ParseRequestLineResult parseRequestLineResult = parseHttpRequestLine(requestTextStream);
        if (parseRequestLineResult.status == PARSED)
        {
            const ParseHeaderResult parseHeaderResult = parseHttpHeaders(requestTextStream);
            if (parseHeaderResult.status == PARSED)
            {
                const GetHostNameResult getHostNameResult = getHostName(parseHeaderResult.value);
                if (getHostNameResult.success)
                {
                    std::istringstream bodyStream(nonEmptyBlocks[1]);
                    const ParseBodyResult parseBodyResult = parseHttpBody(
                        nonEmptyBlocks[1], parseHeaderResult.value,
                        CONFIG.getServer(getHostNameResult.value, socket.port), parseRequestLineResult.value.method);
                    if (parseBodyResult.status == PARSED)
                    {
                        return ParseRequestResult::Success(
                            HttpRequest(parseRequestLineResult.value.method, parseRequestLineResult.value.target,
                                        parseHeaderResult.value, parseBodyResult.value, getHostNameResult.value));
                    }
                    else
                        return ParseRequestResult::Error(parseBodyResult.error);
                }
                else
                    return ParseRequestResult::Error(getHostNameResult.error);
            }
            else if (parseHeaderResult.status == PENDING)
                return ParseRequestResult::Pending();
            else
                return ParseRequestResult::Error(parseHeaderResult.error);
        }
        else
            return ParseRequestResult::Error(parseRequestLineResult.error);
    }
}
