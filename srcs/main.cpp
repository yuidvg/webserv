#include "server/socket.hpp"
#include "config/parse_config.hpp"

int main(int argc, char **argv)
{
   if (argc < 2)
	{
		std::cerr << "config fileを指定してください" << std::endl;
		return 1;
	}
    const char *config_path = argv[1];
	try
	{
        // Configを読む
		Config config(config_path);
		config.DebugPrint();
        // 複数のsocket()を生成
        server();
	}
	catch (const std::runtime_error &e)
	{
		std::cerr << RED << e.what() << NORMAL;
		return 1;
	}
}
