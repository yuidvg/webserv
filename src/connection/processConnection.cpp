#include "../socket/.hpp"
#include "../types/HttpRequestText.hpp"
#include "../httpRequest/.hpp"
#include "../httpResponse/.hpp"
#include ".hpp"

bool processConnection(const Sd &sd, const Servers &servers)
{
    char buffer[500000];
    memset(buffer, 0, sizeof(buffer));
    const int receivedLength = recv(socket.descriptor, buffer, sizeof(buffer) - 1, 0);
    if (receivedLength < 0)
    {
        utils::printError(std::string("recv() failed: " + std::string(strerror(errno))));
        return false;
    }
    else if (receivedLength == 0)
    {
        std::cout << "Connection closed" << std::endl;
        return false;
    }
    std::cout << "Received \n" << receivedLength << " bytes: " << buffer << std::endl;

    HttpRequestText httpRequestText(buffer);
    std::cout << "====server====" << std::endl;
    GetHostNameResult getHostNameResult = httpRequestText.getHostName();
    if (!getHostNameResult.success)
    {
        utils::printError(getHostNameResult.error); // TODO: ここでエラーレスポンス？
        return false;
    }
    Server server = CONFIG.getServer(getHostNameResult.value, socket.port);

    const ParseRequestResult parseHttpRequestResult = parseHttpRequest(httpRequestText);
    const HttpResponse httpResponse = response(parseHttpRequestResult, socket);
    const std::string httpResponseText = responseText(httpResponse);
    const int sentLength = send(sd, httpResponseText.c_str(), httpResponseText.length(), 0);
    if (sentLength > 0)
    {
        return true;
    }
    else
    {
        utils::printError("send() failed.");
        return false;
    }
}
