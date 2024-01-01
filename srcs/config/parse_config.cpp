#include "parse_config.hpp"

void Config::PrintLocation(const Location& location) const
{
	std::cout << "\x1b[33m";
	std::cout << "Location Front_Path: " << location.front_path << std::endl;
	std::cout << "Location Back_Path: " << location.back_path << std::endl;
	std::cout << "Root: " << location.root << std::endl;
	std::cout << "Autoindex: " << (location.autoindex ? "on" : "off") << std::endl;
	std::cout << "Index: " << location.index << std::endl;
	std::cout << "Max Body Size: " << location.client_max_body_size << std::endl;

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
	std::cout << "CGI Executor: " << location.cgi_executor << std::endl;

	// Upload Path
	std::cout << "Upload Path: " << location.upload_path << std::endl;

	// Redirect URL
	std::cout << "Redirect URL: ";
	for (std::map<int, std::string>::const_iterator it = location.redirect.begin();
		it != location.redirect.end(); ++it)
	{
		std::cout << it->first << " => " << it->second << ", ";
	}
	std::cout << std::endl;
	std::cout << "\x1b[0m";
}

void Config::PrintServer(const Server& server) const
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

	std::cout << "Index: " << server.index << std::endl;

	// Locations
	for (size_t i = 0; i < server.locations.size(); ++i)
	{
		std::cout << "\x1b[0m";
		std::cout << "Location #" << (i + 1) << std::endl;
		PrintLocation(server.locations[i]);
	}
	std::cout << "\x1b[0m";
}

void Config::DebugPrint(void) const
{
	std::cout << "server size = " << servers.size() << std::endl;
	// debug
	for (size_t i = 0; i < servers.size(); ++i)
	{
		std::cout << "Server #" << (i + 1) << std::endl;
		PrintServer(servers[i]);
	}
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

// 一般的なテンプレート関数
template <typename T>
T Config::PullWord(std::istringstream& iss)
{
	T word;
	if (!(iss >> word))
	{
		throw std::runtime_error("Config: ディレクティブの引数が不正です: ");
	}
	std::string tmp_str;
	if (iss >> tmp_str)
	{
		std::ostringstream oss;
		oss << "Config: " << word << " ディレクティブの引数が多いです" << std::endl;
		throw(std::runtime_error(oss.str()));
	}
	return word;
}

void Config::HandleErrorPageDirective(std::istringstream& iss, std::map<int, std::string>& error_page)
{
	int error_code; // ステータスコードを取得
	if (!(iss >> error_code))
		throw(std::runtime_error("Config: returnの引数が不正です"));
	std::string error_page_path;
	if (!(iss >> error_page_path))
		throw(std::runtime_error("Config: returnの引数が不正です"));
	error_page[error_code] = error_page_path;

	std::string tmp_str;
	if (iss >> tmp_str)
		throw(std::runtime_error("Config: returnの引数が多いです"));
}

void Config::HandleAllowMethodDirective(std::istringstream& iss, std::vector<std::string>& allow_method)
{
	// 許可されるHTTPメソッドをvectorに追加
	std::string method;
	int i = 0;
	while (iss >> method)
	{
		if (method != "GET" && method != "POST" && method != "DELETE")
			throw(std::runtime_error("Config: 許可されないHTTPメソッドが指定されています"));
		allow_method.push_back(method);
		i++;
	}
	if (i > 3)
		throw(std::runtime_error("Config: 許可されるHTTPメソッドが多すぎます"));
}

void Config::HandleRedirectDirective(std::istringstream& iss, std::map<int, std::string>& redirect)
{
	int status_code; // ステータスコードを取得
	if (!(iss >> status_code))
		throw(std::runtime_error("Config: returnの引数が不正です"));
	std::string redirect_url;
	if (!(iss >> redirect_url))
		throw(std::runtime_error("Config: returnの引数が不正です"));
	std::string tmp_str;
	if (iss >> tmp_str)
		throw(std::runtime_error("Config: returnの引数が多いです"));
	redirect[status_code] = redirect_url; // マップに追加
}

// ロケーションブロックの設定を解析
void Config::ParseLocation(std::ifstream& config_file, Location& location)
{
	std::string line;
	while (std::getline(config_file, line))
	{
		std::istringstream tmp_iss(line);
		std::getline(tmp_iss, line, ';');
		std::istringstream iss(line);
		std::string key;
		iss >> key;
		try
		{
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
				location.index = PullWord<std::string>(iss);
			}
			else if (key == "client_max_body_size")
			{
				location.client_max_body_size = PullWord<size_t>(iss);
			}
			else if (key == "error_page")
			{
				HandleErrorPageDirective(iss, location.error_page);
			}
			else if (key == "allow_method")
			{
				HandleAllowMethodDirective(iss, location.allow_method);
			}
			else if (key == "cgi_executor")
			{
				location.cgi_executor = PullWord<std::string>(iss);
			}
			else if (key == "upload_path")
			{
				location.upload_path = PullWord<std::string>(iss);
			}
			else if (key == "return")
			{
				HandleRedirectDirective(iss, location.redirect);
			}
			else if (key == "}")
			{
				break;
			}
			else if (key.empty() || key == "\t" || key == "\n")
			{
				continue;
			}
			else
			{
				throw std::runtime_error(("不正なディレクティブ"));
			}
		}
		catch (const std::runtime_error& e)
		{
			// 正常に解析が行われなkった部分を発見する
			std::cerr << RED << e.what() << "\nkey: " << key << NORMAL << std::endl;
			throw(std::runtime_error(""));
		}
	}
}

void Config::HandleLocationDirective(std::istringstream& iss, std::ifstream& config_file, Server& server, int type)
{
	Location location;
	location.Initialize();
	std::string tmp_str;
	if (type == FRONT)
	{
		if (!(iss >> location.front_path))
			throw(std::runtime_error("location.front_pathが指定されていません"));
	}
	else
	{
		if (!(iss >> location.back_path))
			throw(std::runtime_error("location.back_pathが指定されていません"));
		if (location.back_path[0] != '.')
			throw(std::runtime_error("location.back_pathは拡張子で指定してください"));
	}
	if (!(iss >> tmp_str) || tmp_str != "{")
		throw(std::runtime_error("Config: locationブロックの開始が不正です"));
	if (iss >> tmp_str)
		throw(std::runtime_error("Config: locationの引数が多いです"));
	ParseLocation(config_file, location);
	server.locations.push_back(location);
	if (config_file.eof())
	{
		throw(std::runtime_error("Config: サーバーブロックが終了していない"));
	}
}
// サーバーブロックの設定を解析
void Config::ParseServer(std::ifstream& config_file, Server& server)
{
	std::string line;
	while (std::getline(config_file, line))
	{
		std::istringstream tmp_iss(line);
		std::getline(tmp_iss, line, ';');
		std::istringstream iss(line);
		std::string key;
		iss >> key;
		try
		{
			// 各ディレクティブに対する処理を記述
			if (key == "location")
			{
				HandleLocationDirective(iss, config_file, server, FRONT);
			}
			else if (key == "location_back")
			{
				HandleLocationDirective(iss, config_file, server, BACK);
			}
			else if (key == "server_name")
			{
				server.server_name = PullWord<std::string>(iss);
			}
			else if (key == "listen")
			{
				server.port = PullWord<int>(iss);
			}
			else if (key == "root")
			{
				server.root = PullWord<std::string>(iss);
			}
			else if (key == "error_page")
			{
				HandleErrorPageDirective(iss, server.error_page);
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
				server.index = PullWord<std::string>(iss);
			}
			else if (key == "}")
			{
				break;
			}
			else if (config_file.eof())
			{
				throw(std::runtime_error("Config: サーバーブロックが終了していない"));
			}
			else if (key.empty() || key == "\t" || key == "\n")
			{
				continue;
			}
			else
			{
				throw std::runtime_error(("Config: 不正なディレクティブ"));
			}
		}
		catch (const std::runtime_error& e)
		{
			std::string next_line;
			std::getline(config_file, next_line);
			std::cout << RED << "次の行: " << next_line << std::endl;
			std::getline(config_file, next_line);
			std::cout << RED << "次の行: " << next_line << std::endl;
			// 正常に解析が行われなkった部分を発見する
			std::cerr << RED << e.what() << "\nkey: " << key << NORMAL << std::endl;
			throw(std::runtime_error(""));
		}
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
			server.InitializeServer();
			std::string tmp_str;
			if (!(iss >> tmp_str) || tmp_str != "{")
				throw(std::runtime_error("Config: serverブロックの開始が不正です"));
			if (iss >> tmp_str)
				throw(std::runtime_error("Config: serverの引数が多いです"));
			ParseServer(config_file, server);
			servers.push_back(server);
		}
	}
}

// int main(int argc, char **argv)
// {
// 	if (argc < 2)
// 	{
// 		std::cerr << "config fileを指定してください" << std::endl;
// 		return 1;
// 	}

// 	const char *config_path = argv[1];
// 	try
// 	{
// 		Config config(config_path);
// 		config.DebugPrint();
// 	}
// 	catch (const std::runtime_error &e)
// 	{
// 		std::cerr << RED << e.what() << NORMAL;
// 		return 1;
// 	}

// 	return 0;
// }
