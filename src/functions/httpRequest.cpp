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
bool isValidRequestLine(const RequestLine requestLine)
{
    if (requestLine.target.find(':') != std::string::npos &&
        requestLine.target.find('*') != std::string::npos) // CONNECT, OPTIONSは非対応
        return false;

    if (requestLine.version != SERVER_PROTOCOL)
        return false;

    return true;
}

ParseRequestLineResult parseHttpRequestLine(std::istringstream &requestTextStream)
{
    // 最初に空行がある場合は読み飛ばす
    std::string firstLine = utils::getlineCustom(requestTextStream);
    while (firstLine.empty() && !requestTextStream.eof())
        firstLine = utils::getlineCustom(requestTextStream);
    if (!firstLine.empty())
        requestTextStream.seekg(0);

    const SplitRequestLineResult splitRequestLineResult = splitRequestLine(requestTextStream);
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

std::vector<std::string> splitBlocks(const std::string &requests)
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
    const std::string requestLineText = requestText.substr(0, requestText.find(CRLF));
    const std::string headersText =
        requestText.substr(requestText.find(CRLF) + 1, requestText.find("\r\n\r\n") - requestText.find(CRLF) - 1);
    const std::string bodyText = requestText.substr(headersText.length() + 4);
    const Strings requestLineParts = utils::split(requestLineText, " ");
    const std::string method = requestLineParts.size() > 0 ? requestLineParts[0] : "";
    const std::string target = requestLineParts.size() > 1 ? requestLineParts[1] : "";
    const StringMap structuredHeader = structureHeaders(headersText);
    const std::string host =
        structuredHeader.find("host") != structuredHeader.end() ? structuredHeader.find("host")->second : "";
    const std::string contentType = structuredHeader.find("content-type") != structuredHeader.end()
                                        ? structuredHeader.find("content-type")->second
                                        : "";
    const std::string contentLength = structuredHeader.find("content-length") != structuredHeader.end()
                                          ? structuredHeader.find("content-length")->second
                                          : "";
    const std::string location =
        structuredHeader.find("location") != structuredHeader.end() ? structuredHeader.find("location")->second : "";
    const std::string allow =
        structuredHeader.find("allow") != structuredHeader.end() ? structuredHeader.find("allow")->second : "";
    const std::string transferEncoding = structuredHeader.find("transfer-encoding") != structuredHeader.end()
                                             ? structuredHeader.find("transfer-encoding")->second
                                             : "";
    return HttpRequest(socket, host, method, target, contentType, contentLength, location, allow, transferEncoding,
                       bodyText);
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
