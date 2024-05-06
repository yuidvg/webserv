#include "../all.hpp"

namespace
{
bool isLineTooLong(const std::string &line)
{
    if (line.length() > MAX_LEN)
        return true;
    else
        return false;
}

/* Parse RequestLine */

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

static SplitRequestLineResult splitRequestLine(std::string &requestLine)
{
    std::string method, target, version;
    std::istringstream requestLineStream(requestLine);

    if (!requestLineStream.eof() && !isLineTooLong(requestLine))
    {
        if ((requestLineStream >> method >> target >> version) && requestLineStream.eof() &&
            validateAndDecodeTarget(target))
            return SplitRequestLineResult::Success(RequestLine(method, target, version));
        else
            return SplitRequestLineResult::Error("Invalid Request Line");
    }
    else
        return SplitRequestLineResult::Error("Invalid Request Line");
}

static bool isValidRequestLine(const RequestLine requestLine)
{
    if (requestLine.target.find(':') != std::string::npos &&
        requestLine.target.find('*') != std::string::npos) // CONNECT, OPTIONSは非対応
        return false;

    if (requestLine.version != SERVER_PROTOCOL)
        return false;

    return true;
}

static ParseRequestLineResult parseHttpRequestLine(std::string &firstLine)
{
    const SplitRequestLineResult splitRequestLineResult = splitRequestLine(firstLine);
    if (splitRequestLineResult.success)
    {
        if (isValidRequestLine(splitRequestLineResult.value))
            return ParseRequestLineResult::Success(splitRequestLineResult.value);
        else
            return ParseRequestLineResult::Error(splitRequestLineResult.error);
    }
    else
        return ParseRequestLineResult::Error(splitRequestLineResult.error);
}

StringMap structureHeaders(std::string headersText)
{
    const Strings headerLines = utils::split(headersText, CRLF);
    StringMap structuredHeader;
    for (Strings::const_iterator it = headerLines.begin(); it != headerLines.end(); ++it)
    {
        const std::string::size_type colonPos = it->find(": ");
        if (colonPos != std::string::npos)
        {
            const std::string key = utils::lowerCase(it->substr(0, colonPos));
            const std::string value = utils::trim(it->substr(colonPos + 1));
            structuredHeader.insert(std::make_pair(key, value));
        }
    }
    return structuredHeader;
}

/* Parse FirstBlock */

static ParseFirstBlockResult parseFirstBlock(const std::string &block)
{
    std::istringstream requestTextStream(block);
    // 最初に空行がある場合は読み飛ばす
    std::string firstLine = utils::getlineCustom(requestTextStream);
    while (firstLine.empty() && !requestTextStream.eof())
        firstLine = utils::getlineCustom(requestTextStream);

    const ParseRequestLineResult parseRequestLineResult = parseHttpRequestLine(firstLine);
    if (parseRequestLineResult.success)
    {
        const ParseHeaderResult parseHeaderResult = parseHeader(requestTextStream);
        if (parseHeaderResult.status == PARSED)
        {
            if (parseHeaderResult.value.find("host") != parseHeaderResult.value.end())
                return ParseFirstBlockResult::Success(
                    FirstBlock(parseRequestLineResult.value, parseHeaderResult.value));
            else
                return ParseFirstBlockResult::Error(block);
        }
        else if (parseHeaderResult.status == PENDING)
            return ParseFirstBlockResult::Pending();
    }
    return ParseFirstBlockResult::Error(block);
}

/* Parse Body */

static UnchunkBodyResult unchunkBody(const std::string &body, const size_t maxBodySize)
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
                    return UnchunkBodyResult::Error("Invalid Body");
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
                            return UnchunkBodyResult::Error("Invalid Body");
                        }
                        else
                        {
                            unchunkedBody += chunks[i];
                        }
                    }
                    else
                        return UnchunkBodyResult::Error("Invalid Body");
                }
                else
                    return UnchunkBodyResult::Error("Invalid Body");
            }
        }
        return UnchunkBodyResult::Error("Invalid Body");
    }
}

static ParseBodyResult parseBody(const std::string &bodyBlock, const Headers &headers, const size_t maxBodySize)
{
    const std::string body = utils::split(bodyBlock, CRLF + CRLF)[0];
    if (headers.find("transfer-encoding") != headers.end())
    {
        if (headers.at("transfer-encoding") == "chunked" && body.length() <= maxBodySize)
        {
            const UnchunkBodyResult unchunkedBodyResult = unchunkBody(body, maxBodySize);
            if (unchunkedBodyResult.status == PARSED)
                return ParseBodyResult::Success(unchunkedBodyResult.value);
            else if (unchunkedBodyResult.status == PENDING)
                return ParseBodyResult::Pending();
            else
                return ParseBodyResult::Error(unchunkedBodyResult.error);
        }
        else
            return ParseBodyResult::Error("Invalid body");
    }
    else if (headers.find("content-length") != headers.end())
    {
        if (utils::isNumber(headers.at("content-length")))
        {
            const size_t bodySize = std::strtoul(headers.at("content-length").c_str(), NULL, 10);
            if ((0 <= bodySize && bodySize <= maxBodySize) && body.length() == bodySize)
                return ParseBodyResult::Success(body);
            else
                return ParseBodyResult::Error("Invalid body");
        }
        else
            return ParseBodyResult::Error("Invalid body");
    }
    else
        return ParseBodyResult::Error("Invalid header");
}

static std::string getHostName(const Headers &headers)
{
    if (headers.find("host") != headers.end())
        return headers.at("host");
    else
        return "";
}

static std::vector<std::string> splitBlocks(const std::string &requests)
{
    std::vector<std::string> blocks = utils::split(requests, CRLF + CRLF);
    for (size_t i = 0; i < blocks.size(); i++)
    {
        blocks[i] += CRLF + CRLF;
    }
    return blocks;
}

HttpRequest parseHttpRequest(const Socket &socket, const std::string &requestText)
{
    const std::vector<std::string> blocks = splitBlocks(request);
    const ParseFirstBlockResult parseFirstBlockResult = parseFirstBlock(blocks[0]);
    if (parseFirstBlockResult.status == PARSED)
    {
        const RequestLine requestLine = parseFirstBlockResult.value.requestLine;
        const std::string host = getHostName(parseFirstBlockResult.value.headers);
        if (!host.empty() && blocks.size() == 2 && blocks[0].find("POST") != std::string::npos)
        {
            const ParseBodyResult parseBodyResult =
                parseBody(blocks[1], parseFirstBlockResult.value.headers,
                          CONFIG.getServer(host, socket.clientPort).clientMaxBodySize);
            if (parseBodyResult.status == PARSED)
                return EventDataOrParsedRequest::Right(HttpRequest(socket, host, requestLine.method, requestLine.target,
                                                                   parseFirstBlockResult.value.headers,
                                                                   parseBodyResult.value));
            else if (parseBodyResult.status == PENDING)
                return EventDataOrParsedRequest::Left(EventData(socket, request));
            else
                return EventDataOrParsedRequest::Right(HttpRequest(socket));
        }
        else
            return EventDataOrParsedRequest::Right(HttpRequest(socket, host, requestLine.method, requestLine.target,
                                                               parseFirstBlockResult.value.headers, ""));
    }
    else if (parseFirstBlockResult.status == PENDING)
        return EventDataOrParsedRequest::Left(EventData(socket, request));
    else
        return EventDataOrParsedRequest::Right(HttpRequest(socket));
}

bool isBody(const std::string &block)
{
    return block.find("POST") == std::string::npos && block.find("GET") == std::string::npos &&
           block.find("DELETE") == std::string::npos;
}

Strings splitHttpRequests(const EventData &eventData)
{
    Strings httpRequests;
    const std::vector<std::string> blocks = splitBlocks(eventData.data);
    for (std::vector<std::string>::const_iterator it = blocks.begin(); it != blocks.end(); ++it)
    {
        if (it->find("POST") != std::string::npos && (it + 1) != blocks.end() && isBody(*(it + 1)))
        {
            const std::string postBlock = *it + *(it + 1);
            httpRequests.push_back(postBlock);
            ++it; // Move to the next block since we've already processed it
        }
        else
        {
            httpRequests.push_back(*it);
        }
    }
    return httpRequests;
}
} // namespace

std::pair<HttpRequests, EventDatas> parseHttpRequests(const EventDatas &httpRequestEventDatas)
{
    HttpRequests httpRequests;
    EventDatas eventDatas;
    for (EventDatas::const_iterator eventDataIt = httpRequestEventDatas.begin();
         eventDataIt != httpRequestEventDatas.end(); ++eventDataIt)
    {
        const Strings requests = splitHttpRequests(*eventDataIt);
        const Strings danglingRequests = utils::exclude(requests, "");
        const Strings completeRequests = utils::exclude(danglingRequests, CRLF + CRLF);
        for (Strings::const_iterator requestIt = requests.begin(); requestIt != requests.end(); ++requestIt)
        {
            const HttpRequest httpRequest = parseHttpRequest(eventDataIt->socket, *requestIt);
            httpRequests.push_back(httpRequest);
        }
    }

    return std::make_pair(httpRequests, eventDatas);
}
