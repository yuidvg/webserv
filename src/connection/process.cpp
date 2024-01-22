#include "process.hpp"
#include "../httpRequest/parse.hpp"
#include "../httpResponse/build.hpp"
#include "../socket/all.hpp"
#include "eventLoop.hpp"

bool processConnection(const Sd &sd)
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
    const ParseRequestResult parseHttpRequestResult = parseHttpRequest(buf);
    const HttpResponse httpResponse = response(parseHttpRequestResult, sd);
    const std::string httpResponseText = responseText(httpResponse);
    const int sentLength = send(sd, utils::toChar(httpResponseText), httpResponseText.length(), 0);
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
