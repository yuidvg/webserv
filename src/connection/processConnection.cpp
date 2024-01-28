#include "../socket/.hpp"
#include "../types/HttpRequestText.hpp"
#include "../httpRequest/.hpp"
#include "../httpResponse/.hpp"
#include ".hpp"

bool processConnection(const Socket &socket)
{
    char buffer[500000];
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
    std::cout << "Received \n" << receivedLength << " bytes: \n" << buffer << std::endl;

    HttpRequestText httpRequestText(buffer, socket);
    const Server server = CONFIG.getServer(httpRequestText.getHostName(), socket.port);

    const ParseRequestResult parseHttpRequestResult = parseHttpRequest(httpRequestText);
    const HttpResponse httpResponse = response(parseHttpRequestResult, socket);
    const std::string httpResponseText = responseText(httpResponse);
    const int sentLength = send(socket.descriptor, httpResponseText.c_str(), httpResponseText.length(), 0);
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
