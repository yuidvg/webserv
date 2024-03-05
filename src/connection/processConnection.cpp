#include "../socket/.hpp"
#include "../httpRequest/.hpp"
#include "../httpResponse/.hpp"
#include "../types/HttpRequestText.hpp"
#include ".hpp"

bool processMessage(const std::string &message)
{
    HttpRequestText httpRequestText(message);

    GetHostNameResult getHostNameResult = httpRequestText.getHostName();
    if (getHostNameResult.success)
    {
        Server server = CONFIG.getServer(getHostNameResult.value, socket.port);

        const ParseRequestResult parseHttpRequestResult = parseHttpRequest(httpRequestText, server);
        const HttpResponse httpResponse = response(parseHttpRequestResult, socket);
        const std::string httpResponseText = responseText(httpResponse);

        const int sentLength = send(socket.descriptor, httpResponseText.c_str(), httpResponseText.length(), 0);
        if (sentLength >= 0)
        {
            return true;
        }
        else
        {
            utils::printError("send() failed.");
            return false;
        }
    }
    else
    {
        utils::printError(getHostNameResult.error);
        return false;
    }
}
