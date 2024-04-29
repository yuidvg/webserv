#include "../all.hpp"

void processHttpMessageFromCgi(const HttpMessage &httpMessage)
{
    if (httpMessage.tag == LEFT)
    {
        HTTP_REQUESTS.push(httpMessage.leftValue);
    }
    else
    {
        const HttpResponse httpResponse = httpMessage.rightValue;
        appendOutbound(httpResponse.destinationSd, stringify(httpResponse));
    }
}

void processClientMessages(const EventDatas &eventDatas)
{
    const ParseHttpRequestResults parsed = parseHttpRequests(eventDatas);
    for (ParseHttpRequestResults::const_iterator it = parsed.begin(); it != parsed.end(); ++it)
    {
        const EventDataOrParsedRequest &parseResult = *it;
        if (parseResult.tag == LEFT)
        {
            const EventData &eventData = parseResult.leftValue;
            //
        }
        else
        {
            const HttpRequest &httpRequest = parseResult.value;
            //
        }
    }
}

void processCgiMessage(const ConnectedUnixSocket &socket, const std::string message)
{
    ParseCgiResponseResult parsed = parseCgiResponse(message, socket.descriptor);
    if (parsed.success)
    {
        const CgiResponse &cgiResponse = parsed.value;
        HttpMessage httpMessageFromCgiResponse = processCgiResponse(cgiResponse);
        if (httpMessageFromCgiResponse.tag == LEFT)
        {
            const HttpRequest &httpRequest = httpMessageFromCgiResponse.leftValue;
            HTTP_REQUESTS.push(httpRequest);
        }
        else
        {
            const HttpResponse &httpResponse = httpMessageFromCgiResponse.rightValue;
            appendOutbound(httpResponse.destinationSd, stringify(httpResponse));
        }
    }
    else
    {
        std::cerr << "Failed to parse CGI response: " << parsed.error << std::endl;
    }
}

void processHttpRequests()
{
    while (!HTTP_REQUESTS.empty())
    {
        const HttpRequest &httpRequest = HTTP_REQUESTS.front();
        CgiRequestOrHttpResponse cgiRequestOrHttpResponse = processHttpRequest(httpRequest);
        if (cgiRequestOrHttpResponse.tag == LEFT)
        {
            const CgiRequest &cgiRequest = cgiRequestOrHttpResponse.leftValue;
            appendOutbound(cgiRequest.destinationSd, cgiRequest.body);
        }
        else
        {
            const HttpResponse &httpResponse = cgiRequestOrHttpResponse.rightValue;
            appendOutbound(httpResponse.destinationSd, stringify(httpResponse));
        }
        HTTP_REQUESTS.pop();
    }
}
