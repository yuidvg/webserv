#include "../socket/.hpp"
#include "../httpRequest/.hpp"
#include "../httpResponse/.hpp"
#include "../types/HttpRequestText.hpp"
#include ".hpp"

ParseStatus processMessage(const Socket &socket)
{
    // Server server = CONFIG.getServer(getHostNameResult.value, socket.port);

    const ParseRequestResult parseHttpRequestResult = parseHttpRequest(socket);
    if (parseHttpRequestResult.status == PENDING)
    {
        return PENDING;
    }
    else
    {
        const HttpResponse httpResponse = response(parseHttpRequestResult, socket);
        const std::string httpResponseText = responseText(httpResponse);

        const int sentLength = send(socket.descriptor, httpResponseText.c_str(), httpResponseText.length(), 0);
        if (sentLength >= 0)
        {
            return PARSED;
        }
        else
        {
            utils::printError("send() failed.");
            return ERROR;
        }
    }
}
