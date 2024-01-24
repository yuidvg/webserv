#include "httpResponse/.hpp"
#include "webserv.hpp"

namespace
{

bool processConnection(const Sd &sd, const Servers &servers)
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

NewListenSdResult newConnectedSd(const Sd listenSd)
{
    const int newSd = accept(static_cast<int>(listenSd), NULL, NULL);
    if (newSd < 0)
    {
        return NewListenSdResult::Error("accept() failed: " + std::string(strerror(errno)));
    }
    std::cout << "New conected socket: " << newSd << std::endl;
    return NewListenSdResult::Success(newSd);
}

void eventLoop(const Sds listenSds, const Servers servers)
{
    Sds connectedSds;

    while (true)
    {
        const Sds allSds = utils::combined(listenSds, connectedSds);
        const ReadableSdsResult readableSdsResult = readableSds(allSds);
        if (!readableSdsResult.success)
        {
            utils::printError(readableSdsResult.error);
        }
        Sds readableSds = readableSdsResult.value;
        for (Sds::iterator readableSdIt = readableSds.begin(); readableSdIt != readableSds.end(); ++readableSdIt)
        {
            if (utils::contains(*readableSdIt, listenSds))
            {
                const Sd listenSd = *readableSdIt;
                NewListenSdResult newConnectedSdResult = newConnectedSd(listenSd);
                if (!newConnectedSdResult.success)
                {
                    utils::printError(newConnectedSdResult.error);
                }
                std::cout << "新しい接続を受け入れた" << std::endl;
                connectedSds.push_back(newConnectedSdResult.value);
            }
            else
            {
                if (!processConnection(*readableSdIt, servers))
                {
                    close(*readableSdIt);
                    connectedSds = utils::excluded(connectedSds, *readableSdIt);
                }
            }
        }
    }
}

} // namespace

int main(int argc, char **argv)
{
    if (argc > 2)
    {
        utils::printError("引数が多すぎます");
        return 1;
    }
    const std::string configPath = argc == 2 ? argv[1] : "config/default.conf";

    ConfigResult configResult = parseConfig::parseConfig(configPath.c_str());
    if (!configResult.success)
    {
        utils::printError(configResult.error);
        return 1;
    }
    const Servers servers = configResult.value;

    GetListenSdsResult createdSdsResult = getListenSds(servers);
    if (!createdSdsResult.success)
    {
        utils::printError(createdSdsResult.error);
        return 1;
    }
    eventLoop(createdSdsResult.value, servers);
}

__attribute__((destructor)) static void destructor(void)
{
    system("leaks -q webserv");
}
