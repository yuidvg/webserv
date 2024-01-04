#include "config.hpp"

void Config::PrintLocation(const Location &location)
{
	std::cout << "\x1b[33m";
	std::cout << "Location Path: " << location.path << std::endl;
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
	std::cout << "CGI Path: " << location.cgi_path << std::endl;

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

void Config::PrintServer(const Server &server)
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

// Configクラスのコンストラクタ
Config::Config(const char *config_path)
{
	ParseConfig(config_path);
}

Config::~Config()
{
	std::cout << "delete config object" << std::endl;
}

void Config::InitializeServer(Server &server)
{
	server.server_name = "";
	server.port = 80;
	server.root = "";
	server.error_page.clear();
	server.client_max_body_size = 1048576; // 1MB
	server.autoindex = false;
	server.index = "index.html";
	server.locations.clear();
}

void Config::InitializeLocation(Location &location)
{
	location.path = "";
	location.root = "";
	location.autoindex = false;
	location.index = "index.html";
	location.client_max_body_size = 0;
	location.error_page.clear();
	location.allow_method.clear();
	location.cgi_path = "";
	location.upload_path = "";
	location.redirect.clear();
}

// 一般的なテンプレート関数
template <typename T>
T Config::PullWord(std::istringstream &iss)
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

// ロケーションブロックの設定を解析
void Config::ParseLocation(std::ifstream &config_file, Location &location)
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
				int error_code; // ステータスコードを取得
				if (!(iss >> error_code))
					throw(std::runtime_error("Config: returnの引数が不正です"));
				std::string error_page_path;
				if (!(iss >> error_page_path))
					throw(std::runtime_error("Config: returnの引数が不正です"));
				location.error_page[error_code] = error_page_path;

				std::string tmp_str;
				if (iss >> tmp_str)
					throw(std::runtime_error("Config: returnの引数が多いです"));
			}
			else if (key == "allow_method")
			{
				// 許可されるHTTPメソッドをvectorに追加
				std::string method;
				int i = 0;
				while (iss >> method)
				{
					if (method != "GET" && method != "POST" && method != "DELETE")
						throw(std::runtime_error("Config: 許可されないHTTPメソッドが指定されています"));
					location.allow_method.push_back(method);
					i++;
				}
				if (i > 3)
					throw(std::runtime_error("Config: 許可されるHTTPメソッドが多すぎます"));
			}
			else if (key == "cgi_path")
			{
				location.cgi_path = PullWord<std::string>(iss);
			}
			else if (key == "upload_path")
			{
				location.upload_path = PullWord<std::string>(iss);
			}
			else if (key == "return")
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
				location.redirect[status_code] = redirect_url; // マップに追加
			}
			else if (key == "}")
			{
				break; // locationブロックの終了
			}
			else if (key.empty() || key == "\t" || key == "\n")
			{
				continue;
			}
			else
			{
				std::string next_line;
				std::getline(config_file, next_line);
				std::cout << RED << "次の行: " << next_line << NORMAL << std::endl;
				std::getline(config_file, next_line);
				std::cout << RED << "次の行: " << next_line << NORMAL << std::endl;
				throw std::runtime_error(("不正なディレクティブ"));
			}
		}
		catch (const std::runtime_error &e)
		{
			// 正常に解析が行われなkった部分を発見する
			std::cerr << RED << e.what() << "\nkey: " << key << NORMAL << std::endl;
			throw(std::runtime_error(""));
		}
	}
}

// サーバーブロックの設定を解析
void Config::ParseServer(std::ifstream &config_file, Server &server)
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
		try
		{
			// 各ディレクティブに対する処理を記述
			if (key == "server_name")
			{
				server.server_name = PullWord<std::string>(iss);
				// if (iss >> key)
				// 	throw(std::runtime_error("Config: server_nameの引数が多いです"));
			}
			else if (key == "listen")
			{
				server.port = PullWord<int>(iss);
			}
			else if (key == "location")
			{
				Location location;
				InitializeLocation(location);
				std::string tmp_str;
				if (!(iss >> location.path))
					throw(std::runtime_error("location.pathが指定されていません"));
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
			else if (key == "root")
			{
				server.root = PullWord<std::string>(iss);
			}
			else if (key == "error_page")
			{
				int error_code;
				if (!(iss >> error_code))
					throw(std::runtime_error("Config: error_pageの引数が不正です"));
				std::string error_page_path;
				if (!(iss >> error_page_path))
					throw(std::runtime_error("Config: error_pageの引数が不正です"));
				server.error_page[error_code] = error_page_path;
				std::string tmp_str;
				if (iss >> tmp_str)
					throw(std::runtime_error("Config: returnの引数が多いです"));
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
				std::string next_line;
				std::getline(config_file, next_line);
				std::cout << RED << "次の行: " << next_line << std::endl;
				std::getline(config_file, next_line);
				std::cout << RED << "次の行: " << next_line << std::endl;
				throw std::runtime_error(("Config: 不正なディレクティブ"));
			}
		}
		catch (const std::runtime_error &e)
		{
			// 正常に解析が行われなkった部分を発見する
			std::cerr << RED << e.what() << "\nkey: " << key << NORMAL << std::endl;
			throw(std::runtime_error(""));
		}
	}
}

// 設定ファイルを解析するメイン関数
void Config::ParseConfig(const char *config_path)
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
			InitializeServer(server);
			std::string tmp_str;
			if (!(iss >> tmp_str) || tmp_str != "{")
				throw(std::runtime_error("Config: serverブロックの開始が不正です"));
			if (iss >> tmp_str)
				throw(std::runtime_error("Config: serverの引数が多いです"));
			ParseServer(config_file, server);
			webserver.servers.push_back(server);
		}
	}
}

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
		Config config(config_path);
		std::cout << "server size = " << config.webserver.servers.size() << std::endl;
		// debug
		for (size_t i = 0; i < config.webserver.servers.size(); ++i)
		{
			std::cout << "Server #" << (i + 1) << std::endl;
			config.PrintServer(config.webserver.servers[i]);
		}
	}
	catch (const std::runtime_error &e)
	{
		std::cerr << RED << e.what() << NORMAL;
		return 1;
	}

	return 0;
}
