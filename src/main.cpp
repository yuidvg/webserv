#include "config/parseConfig.hpp"
#include "server/connection.hpp"
#include "utils/utils.hpp"

int main(int argc, char **argv)
{
    const char *configPath;
    if (argc < 2)
        configPath = "config/default.conf";
    else if (argc == 2)
        configPath = argv[1];
    else
    {
        utils::printError("引数が多すぎます");
        return 1;
    }

    ParseResult result = ParseConfig(configPath);
    if (!result.ok())
    {
        utils::printError(result.unwrapErr());
        return 1;
    }
    std::vector<Server> servers = result.unwrap();

    Connection connection;
    connection.Start(servers);
}

__attribute__((destructor)) static void destructor(void)
{
    system("leaks -q webserv");
}
