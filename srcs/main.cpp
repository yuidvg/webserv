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

	Config config;
	try
	{
		// Configを読む
		config.ParseConfig(config_path);
		config.DebugPrint();
	}
	catch (const std::runtime_error& e)
	{
		std::cerr << RED << e.what() << NORMAL;
		return 1;
	}
	try
	{
		// TODO:複数の仮想サーバーのインスタンスを生成する
		// TODO:下記のコードは仮想サーバーのインスタンスを1つしか生成しないので、修正する必要あり。
		for (size_t i = 0; i <= config.servers.size(); i++)
		{
			std::cout << "server" << i << std::endl;
			Server server;
			server.Start(config.servers[i]);
		}
	}
	catch (const std::runtime_error& e)
	{
		std::cerr << RED << e.what() << NORMAL;
		return 1;
	}
}
