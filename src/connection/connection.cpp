#include "connection.hpp"

NewSocketResult newConnectedSocket(const Socket listenSocket)
{
    const int newSd = accept(listenSocket.descriptor, NULL, NULL);
    if (newSd < 0)
    {
        return NewSocketResult::Error("accept() failed: " + std::string(strerror(errno)));
    }
    std::cout << "New conected socket: " << newSd << std::endl;
    return NewSocketResult::Success(Socket(newSd, listenSocket.server));
}

// 接続が確立されたソケットと通信する
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

unsigned int getMaxSd(const Sockets listenSockets)
{
    unsigned int maxSd = 0;
    for (size_t i = 0; i < listenSockets.size(); ++i)
    {
        if (listenSockets[i].descriptor > static_cast<int>(maxSd))
        {
            maxSd = listenSockets[i].descriptor;
        }
    }
    return maxSd;
}

typedef Result<Sockets, std::string> ReadableSocketsResult;
ReadableSocketsResult readableSockets(const Sockets sockets)
{
    fd_set readableSocketSet = utils::fdSetFrom(sockets);
    std::cout << "Waiting for select()..." << std::endl;
    const int numOfReadableSDs = select(getMaxSd(sockets) + 1, &readableSocketSet, NULL, NULL, NULL);
    if (numOfReadableSDs < 0)
        return ReadableSocketsResult::Error("select() failed: " + std::string(strerror(errno)));
    else if (numOfReadableSDs == 0)
        return ReadableSocketsResult::Error("select() timed out. End program.");
    return ReadableSocketsResult::Success(utils::socketsIn(readableSocketSet, sockets));
}


void startConnection(const Servers servers)
{
    const Sockets listenSockets = getListenSockets(servers);
    eventLoop(listenSockets);
}
