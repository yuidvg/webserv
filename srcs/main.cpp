#include "server/socket.hpp"
#include "config/parse_config.hpp"

int main(int argc, char **argv)
{
	const char *config_path;
	if (argc < 2)
		config_path = "config/default.conf";
	else if (argc == 2)
		config_path = argv[1];
	else
	{
		std::cerr << RED << "引数が多すぎます" << NORMAL << std::endl;
		return 1;
	}

	// Try,Catch構文からResult型に変換する
	// Configを読む
	ParseResult config = ParseConfig(config_path);
	if (!config.ok())
	{
		std::cerr << RED << config.unwrapErr() << NORMAL << std::endl;
		return 1;
	}
	DebugPrint(config.unwrap());

	// Server server;
	// server.Start(config);
}
