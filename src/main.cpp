#include "connection/.hpp"
#include "config/parseConfig.hpp"
#include "socket/.hpp"
#include "webserv.hpp"

Config CONFIG;
namespace
{
bool registerReadEvents(const Sockets &sockets)
{
    for (Sockets::const_iterator socketIt = sockets.begin(); socketIt != sockets.end(); ++socketIt)
    {
        if (!utils::registerEvent(socketIt->descriptor, EVFILT_READ))
        {
            return false;
        }
    }
    return true;
}
} // namespace

int main(int argc, char **argv)
{
    if (KQ != -1)
    {
        if (argc <= 2)
        {
            const std::string configPath = argc == 2 ? argv[1] : "config/default.conf";

            ConfigResult configResult = parseConfig::parseConfig(configPath.c_str());
            if (configResult.success)
            {
                const Servers servers = configResult.value;
                CONFIG.injectServers(servers);
                GetListenSocketsResult createdSocketsResult = getListenSockets(servers);
                if (createdSocketsResult.success)
                {
                    const Sockets &listenSockets = createdSocketsResult.value;
                    if (registerReadEvents(listenSockets))
                        eventLoop(listenSockets);
                    else
                    {
                        utils::printError("failed to register listen sockets to kernel queue.");
                        return 1;
                    }
                }
                else
                {
                    utils::printError(createdSocketsResult.error);
                    return 1;
                }
            }
            else
            {
                utils::printError(configResult.error);
                return 1;
            }
        }
        else
        {
            utils::printError("引数が多すぎます");
            return 1;
        }
    }
    else
    {
        utils::printError("kqueueの作成に失敗しました");
        return 1;
    }
}

// __attribute__((destructor)) static void destructor(void)
// {
//     system("leaks -q webserv");
// }
