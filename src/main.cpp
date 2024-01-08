#include "server/connection.hpp"
#include "config/parseConfig.hpp"
#include "utils/utils.hpp"

int main(int argc, char **argv)
{
	const char *config_path;
	if (argc < 2)
	    config_path = "config/default.conf";
	else if (argc == 2)
		config_path = argv[1];
	else
	{
		utils::printError("引数が多すぎます");
		return 1;
	}

	ParseResult result = ParseConfig(config_path);
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
