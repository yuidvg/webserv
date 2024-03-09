#include "../socket/.hpp"
#include "../httpRequest/.hpp"
#include "../httpResponse/.hpp"
#include ".hpp"

bool processMessage(Socket &socket)
{
    // Server server = CONFIG.getServer(getHostNameResult.value, socket.port);

    const ParseRequestResult parseHttpRequestResult = parseHttpRequest(socket);
    if (parseHttpRequestResult.status == PENDING)
    {
        return false;
    }
    else
    {
        const HttpResponse httpResponse = response(parseHttpRequestResult, socket);
        const std::string httpResponseText = responseText(httpResponse);
        socket.appendToBeSentMessage(httpResponseText);
        return true;
    }
}
