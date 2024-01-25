#include "connection/.hpp"
#include "config/parseConfig.hpp"
#include "socket/.hpp"
#include "webserv.hpp"

Config CONFIG;

int main(int argc, char **argv)
{
    if (argc > 2)
    {
        utils::printError("引数が多すぎます");
        return 1;
    }
    const std::string configPath = argc == 2 ? argv[1] : "CONFIG/default.conf";

    ConfigResult configResult = parseConfig::parseConfig(configPath.c_str());
    if (!configResult.success)
    {
        utils::printError(configResult.error);
        return 1;
    }
    const Servers servers = configResult.value;
    CONFIG.injectServers(servers);
    GetListenSocketsResult createdSocketsResult = getListenSockets(servers);
    if (!createdSocketsResult.success)
    {
        utils::printError(createdSocketsResult.error);
        return 1;
    }
    eventLoop(createdSocketsResult.value);
}

__attribute__((destructor)) static void destructor(void)
{
    system("leaks -q webserv");
}
