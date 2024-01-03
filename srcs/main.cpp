#include "server/socket.hpp"
#include "config/parse_config.hpp"

int main(int argc, char** argv)
{
	const char* config_path;
	if (argc < 2)
		config_path = "config/default.conf";
	else if (argc == 2)
		config_path = argv[1];
	else
	{
		std::cerr << RED << "引数が多すぎます" << NORMAL << std::endl;
		return 1;
	}

	ParseResult result = ParseConfig(config_path);
	if (!result.ok())
	{
		std::cerr << RED << result.unwrapErr() << NORMAL << std::endl;
		return 1;
	}
	std::vector<Server> servers = result.unwrap();
	DebugPrint(servers);

	Connection connection;
	connection.Start(servers);
}
