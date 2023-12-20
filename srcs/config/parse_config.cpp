#include "parse_config.hpp"

void Config::PrintLocation(const Location& location)
{
	std::cout << "\x1b[33m";
	std::cout << "Location Path: " << location.path << std::endl;
	std::cout << "Root: " << location.root << std::endl;
	std::cout << "Autoindex: " << (location.autoindex ? "on" : "off") << std::endl;
	std::cout << "Index: ";
	for (size_t i = 0; i < location.index.size(); ++i)
	{
		std::cout << location.index[i] << (i < location.index.size() - 1 ? ", " : "");
	}
	std::cout << "\nMax Body Size: " << location.client_max_body_size << std::endl;

	// Error Pages
	std::cout << "Error Pages: ";
	for (std::map<int, std::string>::const_iterator it = location.error_page.begin();
		it != location.error_page.end(); ++it)
	{
		std::cout << it->first << " => " << it->second << ", ";
	}
	std::cout << std::endl;

	// Allow Methods
	std::cout << "Allowed Methods: ";
	for (size_t i = 0; i < location.allow_method.size(); ++i)
	{
		std::cout << location.allow_method[i] << (i < location.allow_method.size() - 1 ? ", " : "");
	}
	std::cout << std::endl;

	// CGI Path
	std::cout << "CGI Path: " << location.cgi_path << std::endl;

	// Upload Path
	std::cout << "Upload Path: " << location.upload_path << std::endl;

	// Redirect URL
	std::cout << "Redirect URL: " << location.redirect << std::endl;
	std::cout << "\x1b[0m";
}

void Config::PrintServer(const Server& server)
{
	std::cout << "\x1b[32m";
	std::cout << "Server Name: " << server.server_name << std::endl;
	std::cout << "Port: " << server.port << std::endl;

	// Error Pages
	std::cout << "Server Error Pages: ";
	for (std::map<int, std::string>::const_iterator it = server.error_page.begin();
		it != server.error_page.end(); ++it)
	{
		std::cout << it->first << " => " << it->second << ", ";
	}
	std::cout << std::endl;

	// Max Body Size
	std::cout << "Server Max Body Size: " << server.client_max_body_size << std::endl;

	// Autoindex
	std::cout << "Server Autoindex: " << (server.autoindex ? "on" : "off") << std::endl;

	// Default Index
	std::cout << "Server Default Index: ";
	for (size_t i = 0; i < server.index.size(); ++i)
	{
		std::cout << server.index[i] << (i < server.index.size() - 1 ? ", " : "");
	}
	std::cout << std::endl;

	// Locations
	for (size_t i = 0; i < server.locations.size(); ++i)
	{
		std::cout << "\x1b[0m";
		std::cout << "Location #" << (i + 1) << std::endl;
		PrintLocation(server.locations[i]);
	}
	std::cout << "\x1b[0m";
}

// Configクラスのコンストラクタ
Config::Config(const char* config_path)
{
	ParseConfig(config_path);
}

Config::~Config()
{
	std::cout << "delete config object" << std::endl;
}

void Config::InitializeServer(Server& server)
{
	server.server_name = "";
	server.port = 0;
	server.root = "";
	server.error_page.clear();
	server.client_max_body_size = 0;
	server.autoindex = false;
	server.index.clear();
	server.locations.clear();
}

void Config::InitializeLocation(Location& location)
{
	location.path = "";
	location.root = "";
	location.autoindex = false;
	location.index.clear();
	location.client_max_body_size = 0;
	location.error_page.clear();
	location.allow_method.clear();
	location.cgi_path = "";
	location.upload_path = "";
	location.redirect = "";
}

// 一般的なテンプレート関数
template <typename T>
T Config::PullWord(std::istringstream& iss)
{
	T word;
	if (iss >> word)
	{
		;
	}
	else
	{
		std::cout << "\x1b[31mwordが取得できませんでした\x1b[0m" << std::endl;
		return T();
	}

	return word;
}

// ロケーションブロックの設定を解析
void Config::ParseLocation(std::ifstream& config_file, Location& location)
{
	std::string tmp_line;
	while (std::getline(config_file, tmp_line))
	{
		std::istringstream tmp_iss(tmp_line);
		std::string line;
		std::getline(tmp_iss, line, ';');
		std::istringstream iss(line);
		std::string key;
		iss >> key;

		// 各ディレクティブに対する処理を記述
		if (key == "root")
		{
			location.root = PullWord<std::string>(iss);
		}
		else if (key == "autoindex")
		{
			std::string autoindex_value = PullWord<std::string>(iss);
			location.autoindex = (autoindex_value == "on");
		}
		else if (key == "index")
		{
			location.index.push_back(PullWord<std::string>(iss));
		}
		else if (key == "client_max_body_size")
		{
			location.client_max_body_size = PullWord<size_t>(iss);
		}
		else if (key == "error_page")
		{
			int error_code = PullWord<int>(iss);
			std::string error_page_path = PullWord<std::string>(iss);
			location.error_page[error_code] = error_page_path;
		}
		else if (key == "allow_method")
		{
			// 許可されるHTTPメソッドをvectorに追加
			std::string method;
			while (iss >> method)
			{
				location.allow_method.push_back(method);
			}
		}
		else if (key == "cgi_path")
		{
			location.cgi_path = PullWord<std::string>(iss);
		}
		else if (key == "upload_path")
		{
			location.upload_path = PullWord<std::string>(iss);
		}
		else if (key == "redirect")
		{
			location.redirect = PullWord<std::string>(iss);
		}
		else if (key == "}")
		{
			break; // locationブロックの終了
		}
	}
}

// サーバーブロックの設定を解析
void Config::ParseServer(std::ifstream& config_file, Server& server)
{
	InitializeServer(server);
	std::string tmp_line;
	while (std::getline(config_file, tmp_line))
	{
		std::istringstream tmp_iss(tmp_line);
		std::string line;
		std::getline(tmp_iss, line, ';');
		std::istringstream iss(line);
		std::string key;
		iss >> key;

		// 各ディレクティブに対する処理を記述
		if (key == "server_name")
		{
			server.server_name = PullWord<std::string>(iss);
		}
		else if (key == "listen")
		{
			server.port = PullWord<int>(iss);
		}
		else if (key == "location")
		{
			Location location;
			InitializeLocation(location);
			location.path = PullWord<std::string>(iss);
			ParseLocation(config_file, location);
			server.locations.push_back(location);
		}
		else if (key == "root")
		{
			server.root = PullWord<std::string>(iss);
		}
		else if (key == "error_page")
		{
			int error_code = PullWord<int>(iss);
			std::string error_page_path = PullWord<std::string>(iss);
			server.error_page[error_code] = error_page_path;
		}
		else if (key == "client_max_body_size")
		{
			server.client_max_body_size = PullWord<size_t>(iss);
		}
		else if (key == "autoindex")
		{
			std::string autoindex_value = PullWord<std::string>(iss);
			server.autoindex = (autoindex_value == "on");
		}
		else if (key == "index")
		{
			server.index.push_back(PullWord<std::string>(iss));
		}
		else if (key == "}")
		{
			break;
		}
		// else
		// {
		// 	std::cout << "\x1b[31m不正なディレクティブです\x1b[0m" << std::endl;
		// 	return;
		// }
	}
}

// 設定ファイルを解析するメイン関数
void Config::ParseConfig(const char* config_path)
{
	std::ifstream config_file(config_path);
	std::string line;
	if (!config_file.is_open())
	{
		std::cerr << "Failed to open file: " << config_path << std::endl;
		return;
	}

	while (std::getline(config_file, line))
	{
		std::istringstream iss(line);
		std::string key;
		iss >> key;

		if (key == "server")
		{
			Server server;
			ParseServer(config_file, server);
			webserver.servers.push_back(server);
		}
	}
}

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		std::cerr << "config fileを指定してください" << std::endl;
		return 1;
	}

	const char* config_path = argv[1];
	Config config(config_path);
	std::cout << "server size = " << config.webserver.servers.size() << std::endl;
	// debug
	for (size_t i = 0; i < config.webserver.servers.size(); ++i)
	{
		std::cout << "Server #" << (i + 1) << std::endl;
		config.PrintServer(config.webserver.servers[i]);
	}

	return 0;
}
