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

    ParseResult result = parseConfig(configPath);
    if (!result.ok())
    {
        utils::printError(result.unwrapErr());
        return 1;
    }
    const std::vector<Server> servers = result.unwrap();

    StartConnection(servers);
}

__attribute__((destructor)) static void destructor(void)
{
    system("leaks -q webserv");
}
