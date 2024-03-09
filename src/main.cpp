#include "connection/.hpp"
#include "config/parseConfig.hpp"
#include "socket/.hpp"
#include "webserv.hpp"

Config CONFIG;

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
                    eventLoop(createdSocketsResult.value);
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
