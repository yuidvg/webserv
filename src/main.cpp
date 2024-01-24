#include "config/parseConfig.hpp"
#include "connection/.hpp"
#include "socket/.hpp"
#include "webserv.hpp"

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
