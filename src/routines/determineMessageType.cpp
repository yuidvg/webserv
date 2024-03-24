#include "../all.hpp"

namespace
{

} // namespace

void handleReceivedMessage(std::string message, void (*clearMessage)(void), const ConnectedSocket &socket)
{
}

void handleClientMessages(const ConnectedInternetSocket &socket, const SocketBuffer &socketBuffer)
{
    // messageをパースして、httpRequestsに変換
    ParseHttpRequests parseHttpRequests(socketBuffer, socket.serverPort);
    if (parseHttpRequests.tag == RIGHT)
    {
        // httpRequestsを処理
        ;
    }
    else
    {
        // pending
        return;
    }
}
