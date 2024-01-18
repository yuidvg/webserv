#include "process.hpp"
#include "../httpRequest/parse.hpp"
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
    ParseRequestResult parseHttpRequestResult = parseHttpRequest(buf, socket.server);
    if (parseHttpRequestResult.success)
    {
        const std::string response = "HTTP/1.1 200 OK\r\nContent-Length: 22\r\n\r\n<h1>Hello Webserv</h1>";
        const int sentLength = send(socket.descriptor, response.c_str(), response.length(), 0);
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
    else
    {
        utils::printError(std::string("parseHttpRequest() failed\nstatus code: " +
                                      utils::to_string((parseHttpRequestResult.error.statusCode))));
        return false;
    }
}
