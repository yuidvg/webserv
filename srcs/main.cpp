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
	try
	{
		// Configを読む
		Config config(config_path);
		config.DebugPrint();
		// 複数のsocket()を生成
		server();
	}
	catch (const std::runtime_error& e)
	{
		std::cerr << RED << e.what() << NORMAL;
		return 1;
	}
}
