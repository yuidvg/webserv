#include "../socket/.hpp"
#include "../httpRequest/.hpp"
#include "../httpResponse/.hpp"
#include ".hpp"

bool processConnection(const Socket &sd, const Servers &servers)
{
    char buffer[500000];
    const int receivedLength = recv(sd, buffer, sizeof(buffer) - 1, 0);
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

    std::istringstream buf(buffer);
    const ParseRequestResult parseHttpRequestResult = parseHttpRequest(buf, servers, sd);
    const HttpResponse httpResponse = response(parseHttpRequestResult, sd, servers);
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
