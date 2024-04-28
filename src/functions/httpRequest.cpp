#include "../all.hpp"

/* ========  helper functions ======== */

static std::vector<std::string> splitBlocks(const std::string &requests)
{
    std::vector<std::string> blocks = utils::split(requests, CRLF + CRLF);
    for (size_t i = 0; i < blocks.size(); i++)
    {
        blocks[i] += CRLF + CRLF;
    }
    return blocks;
}

bool isBody(const std::string &block)
{
    return block.find("POST") == std::string::npos && block.find("GET") == std::string::npos && block.find("DELETE") == std::string::npos;
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
    if (body.find("\r\n0") == std::string::npos)
    {
        return UnchunkBodyResult::Pending();
    }
    else
    {
        const std::vector<std::string> chunks = utils::split(body, CRLF);
        unsigned long chunkSize;
        std::string unchunkedBody = "";
        for (size_t i = 0; i < chunks.size(); i++)
        {
            if (i % 2 == 0)
            {
                if (!utils::isHex(chunks[i]))
                    return UnchunkBodyResult::Error(BAD_REQUEST);
                chunkSize = std::strtoul(chunks[i].c_str(), NULL, 16);
                if (chunkSize == 0)
                    return UnchunkBodyResult::Success(unchunkedBody);
            }
            else
            {
                if (chunks[i].length() <= maxBodySize)
                {
                    if (chunks[i].length() == chunkSize)
                    {
                        if (i == chunks.size() - 1 || unchunkedBody.length() + chunkSize > maxBodySize)
                        {
                            return UnchunkBodyResult::Error(BAD_REQUEST);
                        }
                        else
                        {
                            unchunkedBody += chunks[i];
                        }
                    }
                    else
                        return UnchunkBodyResult::Error(BAD_REQUEST);
                }
                else
                    return UnchunkBodyResult::Error(BAD_REQUEST);
            }
        }
        return UnchunkBodyResult::Error(BAD_REQUEST);
    }
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
    // tellgで現在の位置を取得、その先からCRLFまでstringを取得する
    const std::streampos currentPos = requestTextStream.tellg();
    std::string requestStr = requestTextStream.str();
    const std::size_t headerEndPos = requestStr.find("\r\n\r\n", currentPos);
    const std::streampos endPos = (headerEndPos != std::string::npos) ? headerEndPos + 4 : requestStr.length();
    const std::string headerText = requestStr.substr(currentPos, endPos - currentPos);

    std::vector<std::string> header = utils::split(headerText, CRLF);
    if (header.size() == 0)
        return ParseHeaderResult::Pending();
    else
    {
        Headers headers;
        for (std::vector<std::string>::iterator it = header.begin(); it != header.end(); ++it)
        {
            std::string::size_type pos = it->find(": ");
            if (pos != std::string::npos)
            {
                std::string key = it->substr(0, pos);
                std::string value = it->substr(pos + 1);
                std::transform(key.begin(), key.end(), key.begin(), ::tolower); // headerのkeyは大文字小文字を区別しない
                value = utils::trim(value);                                     // valueの前後の空白を削除
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
            if (unchunkedBodyResult.status == PARSED)
                return ParseBodyResult::Success(unchunkedBodyResult.value);
            else if (unchunkedBodyResult.status == PENDING)
                return ParseBodyResult::Pending();
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
            const long tmp = std::strtol(headers.at("content-length").c_str(), NULL, 10);
            if (tmp < 0 || static_cast<size_t>(tmp) > std::numeric_limits<size_t>::max())
                return ParseBodyResult::Error(BAD_REQUEST);
            const size_t bodySize = static_cast<size_t>(tmp);
            if ((0 <= bodySize && bodySize <= server.clientMaxBodySize) && body.length() == bodySize)
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

EventDataOrParsedRequest parseHttpRequest(const int sd, const std::string &request)
{
    const std::string blocks = splitBlocks(request);
    const ParseFirstBlockResult parseFirstBlockResult = parseFirstBlock(blocks[0]);
    if(parseFirstBlockResult.status == Parsed)
    {
        const GetHostNameResult getHostNameResult = getHostName(parseFirstBlockResult.value.headers);
        if ()
        {
            const ParseBodyResult parseBodyResult = parseHttpBody(parseFirstBlockResult.value.body, parseFirstBlockResult.value.headers, CONFIG.getServer(host, port), parseFirstBlockResult.value.requestLine.method);
        }
        else
            return EventDataOrParsedRequest::Right(HttpRequest());
    }
    else if (parseFirstBlockResult.status == Pending)
        return EventDataOrParsedRequest::Left(EventData(sd, request));
    else
        return EventDataOrParsedRequest::Right(HttpRequest());
}

static std::vector<std::string> splitHttpRequests(const EventData &eventData)
{
    std::vector<std::string> httpRequests;
    const std::vector<std::string> blocks = splitBlocks(eventData.data);
    for (size_t i = 0; i < blocks.size(); i++)
    {
        if(blocks[i].find("POST") != std::string::npos && i != blocks.size() - 1 && isBody(blocks[i + 1]))
        {
            const std::string postBlock = blocks[i] + blocks[i + 1];
            httpRequests.push_back(postBlock);
            i++;
        }
        else
            httpRequests.push_back(blocks[i]);
    }
    return httpRequests;
}

ParseHttpRequestResults ParseHttpRequestResults(const EventDatas &httpRequestEventDatas)
{
    ParseHttpRequestResults parseHttpRequestResults;
    for (size_t i = 0; i < httpRequestEventDatas.size(); i++)
    {
        const HttpRequests httpRequests = splitHttpRequests(httpRequestEventDatas[i]);
        for (size_t j = 0; j < httpRequests.size(); j++)
        {
            const ParseHttpRequestResults parseHttpRequestResult = parseHttpRequest(httpRequestEventDatas[i].sd, httpRequests[j]);
            parseHttpRequestResults.push_back(parseHttpRequestResult);
        }
    }
    return parseRequestResults;
}

// {
//     const std::vector<std::string> blocks = splitHttpRequests(socketBuffer.getInbound());
//     size_t size = 0;
//     std::queue<const HttpRequest> httpRequests;
//     for (size_t i = 0; i < blocks.size(); i++)
//     {
//         const ParseRequestResult parseRequestResult = parseHttpRequest(blocks[i], socket);
//         if (parseRequestResult.status == PARSED)
//         {
//             httpRequests.push(parseRequestResult.value);
//             size += blocks[i].length();
//             continue;
//         }
//         else if (parseRequestResult.status == PENDING)
//         {
//             // POSTの場合もここに来る
//             if (i != blocks.size() - 1 && blocks[i].find("POST") != std::string::npos)
//             {
//                 const std::string postBlock = blocks[i] + blocks[i + 1];
//                 const ParseRequestResult parseRequestResult = parseHttpRequest(postBlock, socket);
//                 if (parseRequestResult.status == PARSED)
//                 {
//                     httpRequests.push(parseRequestResult.value);
//                     size += blocks[i + 1].length();
//                     i++;
//                 }
//                 else if (parseRequestResult.status == ERROR)
//                 {
//                     HTTP_REQUESTS.push(parseRequestResult.error);
//                     size += blocks[i].length();
//                 }
//             }
//             return ParsedHttpRequests(httpRequests, size);
//         }
//         else
//         {
//             HTTP_REQUESTS.push(parseRequestResult.error);
//         }
//     }
//     return ParsedHttpRequests(httpRequests, size);
// }
