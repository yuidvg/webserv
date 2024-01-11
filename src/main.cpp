#include "config/config.hpp"
#include "server/connection.hpp"
#include "utils/utils.hpp"

int main(int argc, char **argv)
{
    if (argc > 2)
    {
        utils::printError("引数が多すぎます");
        return 1;
    }
    const std::string configPath = argc == 2 ? argv[1] : "config/default.conf";

    ConfigResult configResult = parseConfig(configPath.c_str());
    if (!configResult.success)
    {
        utils::printError(configResult.error);
        return 1;
    }
    const std::vector<Server> servers = configResult.value;

    Connection connection;
    connection.Start(servers);
}

__attribute__((destructor)) static void destructor(void)
{
    system("leaks -q webserv");
}
