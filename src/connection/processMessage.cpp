#include "../socket/.hpp"
#include "../httpRequest/.hpp"
#include "../httpResponse/.hpp"
#include ".hpp"

std::string processedMessage(Connection &connection)
{
    // Server server = CONFIG.getServer(getHostNameResult.value, connection.port);

    const ParseRequestResult parseHttpRequestResult = parseHttpRequest(connection);
    if (parseHttpRequestResult.status == PENDING)
    {
        return false;
    }
    else
    {
        const HttpResponse httpResponse = response(parseHttpRequestResult, connection);
        const std::string httpResponseText = responseText(httpResponse);
        return connection.appendToBeSentMessage(httpResponseText);
    }
}
