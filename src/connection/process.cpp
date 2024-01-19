#include "process.hpp"
#include "../httpRequest/parse.hpp"
#include "../httpResponse/build.hpp"
#include "../socket/all.hpp"
#include "eventLoop.hpp"

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
    std::cout << "Received \n" << receivedLength << " bytes: " << buffer << std::endl;

    std::istringstream buf(buffer);
    ParseRequestResult parseHttpRequestResult = parseHttpRequest(buf);
    const HttpResponse httpResponse = response(parseHttpRequestResult, socket.server);
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
