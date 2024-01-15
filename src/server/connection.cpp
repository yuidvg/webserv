#include "connection.hpp"

void closeConnection(int sd, int &maxSd, fd_set &masterSet, std::map<int, Socket> &connSocks)
{
    std::cout << "close sd: " << sd << std::endl;

    close(sd);
    deleteConnSock(sd, connSocks);
    FD_CLR(sd, &masterSet);
    if (sd == maxSd)
    {
        while (maxSd > 0 && !FD_ISSET(maxSd, &masterSet))
        {
            maxSd -= 1;
        }
        if (maxSd == 0 && !FD_ISSET(0, &masterSet))
        {
            // 有効なディスクリプタが残っていない
            maxSd = -1;
        }
    }
}

void allcloseConnection(int &maxSd, fd_set &masterSet, std::map<int, Socket> &connSocks)
{
    for (int i = 0; i <= maxSd; ++i)
    {
        if (FD_ISSET(i, &masterSet))
        {
            closeConnection(i, maxSd, masterSet, connSocks);
        }
    }
}

NewSDResult acceptNewConnection(const int listenSd, int &maxSd, fd_set &masterSet)
{
    int newSd = -1;
    newSd = accept(listenSd, NULL, NULL);
    if (newSd < 0)
    {
        if (errno != EWOULDBLOCK)
        {
            std::string errorMsg = "accept() failed on socket ";
            return NewSDResult::Error(errorMsg);
        }
    }
    std::cout << "New incoming connection " << newSd << std::endl;
    FD_SET(newSd, &masterSet);
    if (newSd > maxSd)
    {
        maxSd = newSd;
    }
    return NewSDResult::Success(newSd);
}

void deleteConnSock(int sd, std::map<int, Socket> &connSocks)
{
    connSocks.erase(sd);
}

// 接続が確立されたソケットと通信する
void processConnection(int sd, Socket &socket, int &maxSd, fd_set &masterSet, std::map<int, Socket> &connSocks)
{
    char buffer[500000] = {0}; // Initialize buffer with null
    int len;

    len = recv(sd, buffer, sizeof(buffer) - 1, 0);
    if (len < 0)
    {
        utils::printError(std::string("recv() failed: " + std::string(strerror(errno))));
        closeConnection(sd, maxSd, masterSet, connSocks);
        return;
    }
    else if (len == 0)
    {
        std::cout << "Connection closed" << std::endl;
        closeConnection(sd, maxSd, masterSet, connSocks);
        return;
    }
    std::cout << "Received \n" << len << " bytes: " << buffer << std::endl;

    // (void)socket;
    std::istringstream buf(buffer);
    // TODO:parseHttpRequestの第３引数にserversを渡したい。
    ParseRequestResult parserResult = parseHttpRequest(buf, socket.getServer());
    if (!parserResult.success)
    {
        utils::printError(std::string("parseHttpRequest() failed\nstatus code: " +
                                      utils::to_string((parserResult.error.statusCode))));
        closeConnection(sd, maxSd, masterSet, connSocks);
        return;
    }

    // TODO: HTTPレスポンスを作成する
    // HTTPレスポンス作成のロジックをここに実装
    std::string response = "HTTP/1.1 200 OK\r\nContent-Length: 22\r\n\r\n<h1>Hello Webserv</h1>";
    len = send(sd, response.c_str(), response.length(), 0);
    if (len < 0)
    {
        utils::printError("send() failed: ");
        closeConnection(sd, maxSd, masterSet, connSocks);
        return;
    }
}

void StartConnection(const std::vector<Server> servers)
{
    std::vector<int> listenSockets;
    std::vector<Socket> sockets;
    int maxSd = -1;
    fd_set masterSet;
    fd_set workingSet;
    std::map<int, Socket> connSocks;
    bool samePort = false;

    // 各仮想サーバーのソケットを初期化し、監視セットに追加
    FD_ZERO(&masterSet);
    for (size_t i = 0; i < servers.size(); ++i)
    {
        for (size_t j = 0; j < i; ++j)
        {
            if (servers[i].port == servers[j].port)
            {
                utils::printError(std::string("ポート番号" + utils::to_string(servers[i].port) +
                                              "はすでに使用されているため、liste socketは作成しません"));
                samePort = true;
                break;
            }
        }
        if (samePort)
        {
            samePort = false;
            continue;
        }
        Socket socket(servers[i]);
        sockets.push_back(socket);
        int listenSd = socket.getListenSocket();
        listenSockets.push_back(listenSd);
        FD_SET(listenSd, &masterSet);
        if (listenSd > maxSd)
        {
            maxSd = listenSd;
        }
    }

    struct timeval timeout;
    timeout.tv_sec = 3 * 60;
    timeout.tv_usec = 0;
    while (true)
    {
        memcpy(&workingSet, &masterSet, sizeof(masterSet));

        std::cout << "Waiting on select()..." << std::endl;
        int rc = select(maxSd + 1, &workingSet, NULL, NULL, &timeout);
        if (rc < 0)
        {
            utils::printError(std::string("select() failed: " + std::string(strerror(errno))));
            break;
        }
        else if (rc == 0)
        {
            std::cout << "select() timed out. End program." << std::endl;
            break;
        }

        int descReady = rc;
        for (int i = 0; i <= maxSd && descReady > 0; ++i)
        {
            if (FD_ISSET(i, &workingSet))
            {
                // 読み取り可能なfdが見つかったら、検索を停止するため。
                descReady -= 1;
                // リスニングソケットの確認
                if (std::find(listenSockets.begin(), listenSockets.end(), i) != listenSockets.end())
                {
                    NewSDResult newSDResult = acceptNewConnection(i, maxSd, masterSet);
                    if (!newSDResult.success)
                    {
                        utils::printError(newSDResult.error);
                        allcloseConnection(maxSd, masterSet, connSocks);
                        return;
                    }
                    for (size_t index = 0; index < sockets.size(); ++index)
                    {
                        if (sockets[index].getListenSocket() == i)
                        {
                            std::cout << "新しい接続を受け入れた" << std::endl;
                            connSocks.insert(std::make_pair(newSDResult.value, sockets[index]));
                        }
                    }
                }
                else
                {
                    processConnection(i, connSocks[i], maxSd, masterSet, connSocks);
                }
            }
        }
    }
    std::cout << "Closing socket discriptor..." << std::endl;
    allcloseConnection(maxSd, masterSet, connSocks);
}
