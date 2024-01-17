#include "connection.hpp"

void closeConnection(int sd, fd_set &masterSet, std::map<int, Socket> &connSocks)
{
    std::cout << "close sd: " << sd << std::endl;

    close(sd);
    deleteConnSock(sd, connSocks);
    FD_CLR(sd, &masterSet);
    // if (sd == maxSd)
    // {
    //     while (maxSd > 0 && !FD_ISSET(maxSd, &masterSet))
    //     {
    //         maxSd -= 1;
    //     }
    //     if (maxSd == 0 && !FD_ISSET(0, &masterSet))
    //     {
    //         // 有効なディスクリプタが残っていない
    //         maxSd = -1;
    //     }
    // }
}

void allcloseConnection(fd_set &masterSet, Sockets &sockets,std::map<int, Socket> &connSockets)
{
    for (int i = 0; i <= getMaxSd(sockets,connSockets); ++i)
    {
        if (FD_ISSET(i, &masterSet))
        {
            closeConnection(i, masterSet, connSockets);
        }
    }
}

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

void deleteConnSock(int sd, std::map<int, Socket> &connSocks)
{
    connSocks.erase(sd);
}

// 接続が確立されたソケットと通信する
void processConnection(int sd, Socket &socket, fd_set &masterSet, std::map<int, Socket> &connSocks)
{
    char buffer[500000] = {0}; // Initialize buffer with null
    int len;

    len = recv(sd, buffer, sizeof(buffer) - 1, 0);
    if (len < 0)
    {
        utils::printError(std::string("recv() failed: " + std::string(strerror(errno))));
        closeConnection(sd, masterSet, connSocks);
        return;
    }
    else if (len == 0)
    {
        std::cout << "Connection closed" << std::endl;
        closeConnection(sd, masterSet, connSocks);
        return;
    }
    std::cout << "Received \n" << len << " bytes: " << buffer << std::endl;

    std::istringstream buf(buffer);
    // TODO:parseHttpRequestの第３引数にserversを渡したい。
    ParseRequestResult parserResult = parseHttpRequest(buf, socket.server);
    if (!parserResult.success)
    {
        utils::printError(std::string("parseHttpRequest() failed\nstatus code: " +
                                      utils::to_string((parserResult.error.statusCode))));
        closeConnection(sd, masterSet, connSocks);
        return;
    }

    // TODO: HTTPレスポンスを作成する
    // HTTPレスポンス作成のロジックをここに実装
    std::string response = "HTTP/1.1 200 OK\r\nContent-Length: 22\r\n\r\n<h1>Hello Webserv</h1>";
    len = send(sd, response.c_str(), response.length(), 0);
    if (len < 0)
    {
        utils::printError("send() failed: ");
        closeConnection(sd, masterSet, connSocks);
        return;
    }
}

int getMaxSd(const Sockets sockets)
{
    int maxSd = 0;
    for (size_t i = 0; i < sockets.size(); ++i)
    {
        if (sockets[i].descriptor > maxSd)
        {
            maxSd = sockets[i].descriptor;
        }
    }
    return maxSd;
}

fd_set fdSetFrom(const Sockets sockets)
{
    fd_set fdSet;
    FD_ZERO(&fdSet);
    for (size_t i = 0; i < sockets.size(); ++i)
    {
        FD_SET(sockets[i].listenDescriptor, &fdSet);
    }
    return fdSet;
}

Sockets socketsIn(const fd_set fdSet, const Sockets sockets)
{
    Sockets socketsFdSet;
    for (size_t i = 0; i < sockets.size(); ++i)
    {
        if (FD_ISSET(sockets[i].listenDescriptor, &fdSet))
        {
            socketsFdSet.push_back(sockets[i]);
        }
    }
    return socketsFdSet;
}

typedef Result<Sockets, std::string> ReadableSocketsResult;
ReadableSocketsResult readableSockets(const Sockets sockets)
{
    fd_set readableSocketSet = fdSetFrom(sockets);
    const int numOfReadableSDs = select(getMaxSd(sockets) + 1, &readableSocketSet, NULL, NULL, NULL);
    if (numOfReadableSDs < 0)
        return ReadableSocketsResult::Error("select() failed: " + std::string(strerror(errno)));
    else if (numOfReadableSDs == 0)
        return ReadableSocketsResult::Error("select() timed out. End program.");
    return ReadableSocketsResult::Success(socketsIn(readableSocketSet, sockets));
}

void eventLoop(Sockets &sockets)
{
    fd_set masterSet;
    fd_set readableSocketSet;
    std::map<int, Socket> connDiscriptors;


    while (true)
    {
        memcpy(&readableSocketSet, &masterSet, sizeof(masterSet));

        std::cout << "Waiting on select()..." << std::endl;
        const ReadableSocketsResult readableSocketsResult = readableSockets(sockets);
        if (!readableSocketsResult.success)
        {
            utils::printError(readableSocketsResult.error);
            // ToDo: close all sockets/not.
        }
        const Sockets readableSockets = readableSocketsResult.value;
        for (int i = 0; i <= getMaxSd(readableSockets); ++i)
        {
            if (FD_ISSET(i, &readableSocketSet))
            {
                if (std::find(sockets.begin(), sockets.end(), i) != sockets.end())
                // if (std::find(listenSockets.begin(), listenSockets.end(), i) != listenSockets.end())
                //　新たに接続するSocket
                {
                    NewSocketResult newSDResult = newConnectedSocket(i, masterSet);
                    if (!newSDResult.success)
                    {
                        utils::printError(newSDResult.error);
                        allcloseConnection(masterSet,sockets, connDiscriptors);
                        break;
                    }
                    for (size_t index = 0; index < sockets.size(); ++index)
                    {
                        if (sockets[index].listenDescriptor == i)
                        {
                            std::cout << "新しい接続を受け入れた" << std::endl;
                            connDiscriptors.insert(std::make_pair(newSDResult.value, sockets[index]));
                        }
                    }
                }
                else
                {
                    // connectedSocket
                    processConnection(i, connDiscriptors[i], masterSet, connDiscriptors);
                }
            }
        }
    }

}


void startConnection(const std::vector<Server> servers)
{
    Sockets sockets;
    for (size_t i = 0; i < servers.size(); ++i)
    {
        Socket socket(servers[i]);
        sockets.push_back(socket);
    }
    eventLoop(sockets);
}