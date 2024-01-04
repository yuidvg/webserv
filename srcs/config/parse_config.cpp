#include "parse_config.hpp"

// 一般的なテンプレート関数
template <typename T>
Result<T, std::string> PullWord(std::istringstream& iss)
{
	T word;
	if (!(iss >> word))
	{
		return Result<T, std::string>::Err("Config: ディレクティブの引数が不正です");
	}
	std::string tmp_str;
	if (iss >> tmp_str)
	{
		std::ostringstream oss;
		oss << "Config: " << word << " ディレクティブの引数が多いです";
		return Result<T, std::string>::Err(oss.str());
	}
	return Result<T, std::string>::Ok(word);
}

ErrorPageMapResult HandleErrorPageDirective(std::istringstream& iss)
{
	std::map<int, std::string> error_page;
	int error_code; // ステータスコードを取得
	std::string error_page_path;

	if (!(iss >> error_code))
		return ErrorPageMapResult::Err("Config: error_code引数が不正です");
	if (!(iss >> error_page_path))
		return ErrorPageMapResult::Err("Config: error_page_path");
	error_page[error_code] = error_page_path;
	std::string tmp_str;
	if (iss >> tmp_str)
		return ErrorPageMapResult::Err("Config: error_pageの引数が多いです");
	return ErrorPageMapResult::Ok(error_page);
}

// ロケーションブロックの設定を解析
ParseRoutesResult ParseLocation(std::ifstream& config_file, Location& location)
{
	std::string line;

	while (std::getline(config_file, line))
	{
		std::istringstream tmp_iss(line);
		std::getline(tmp_iss, line, ';');
		std::istringstream iss(line);
		std::string key;
		iss >> key;
		if (key == "root")
		{
			Result<std::string, std::string> result = PullWord<std::string>(iss);
			if (!result.ok())
				return ParseRoutesResult::Err(result.unwrapErr());
			location.root = result.unwrap();
		}
		else if (key == "autoindex")
		{
			Result<std::string, std::string> result = PullWord<std::string>(iss);
			if (!result.ok())
				return ParseRoutesResult::Err(result.unwrapErr());
			location.autoindex = (result.unwrap() == "on");
		}
		else if (key == "index")
		{
			Result<std::string, std::string> result = PullWord<std::string>(iss);
			if (!result.ok())
				return ParseRoutesResult::Err(result.unwrapErr());
			location.index = result.unwrap();
		}
		else if (key == "client_max_body_size")
		{
			Result<size_t, std::string> result = PullWord<size_t>(iss);
			if (!result.ok())
				return ParseRoutesResult::Err(result.unwrapErr());
			location.client_max_body_size = result.unwrap();
		}
		else if (key == "error_page")
		{
			ErrorPageMapResult errorPageMapResult = HandleErrorPageDirective(iss);
			if (!errorPageMapResult.ok())
				return ParseRoutesResult::Err(errorPageMapResult.unwrapErr());
			location.error_page = errorPageMapResult.unwrap();
		}
		else if (key == "allow_method")
		{
			// 許可されるHTTPメソッドをvectorに追加
			std::string method;
			int i = 0;
			while (iss >> method)
			{
				if (method != "GET" && method != "POST" && method != "DELETE")
					return (ParseRoutesResult::Err("Config: 許可されないHTTPメソッドが指定されています"));
				location.allow_method.push_back(method);
				i++;
			}
			if (i > 3)
				return (ParseRoutesResult::Err("Config: 許可されるHTTPメソッドが多すぎます"));
		}
		else if (key == "cgi_executor")
		{
			std::string cgi_program;
			while (iss >> cgi_program)
			{
				location.cgi_executor.push_back(cgi_program);
			}
		}
		else if (key == "upload_path")
		{
			Result<std::string, std::string> result = PullWord<std::string>(iss);
			if (!result.ok())
				return ParseRoutesResult::Err(result.unwrapErr());
			location.upload_path = result.unwrap();
		}
		else if (key == "return")
		{
			int status_code; // ステータスコードを取得
			if (!(iss >> status_code))
				return ParseRoutesResult::Err("Config: returnの引数が不正です");
			std::string redirect_url;
			if (!(iss >> redirect_url))
				return ParseRoutesResult::Err("Config: returnの引数が不正です");
			std::string tmp_str;
			if (iss >> tmp_str)
				return ParseRoutesResult::Err("Config: returnの引数が多いです");
			location.redirect[status_code] = redirect_url; // マップに追加
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
			return ParseRoutesResult::Err("不正なディレクティブ");
		}
	}
	return ParseRoutesResult::Ok("OK");
}

// サーバーブロックの設定を解析
ParseServerResult ParseServer(std::ifstream& config_file)
{
	std::string line;
	Server server;
	while (std::getline(config_file, line))
	{
		std::istringstream tmp_iss(line);
		std::getline(tmp_iss, line, ';');
		std::istringstream iss(line);
		std::string key;
		iss >> key;
		// 各ディレクティブに対する処理を記述
		// TODO:switch文に変更
		if (key == "location")
		{
			Location location;
			std::string tmp_str;
			if (!(iss >> location.front_path))
				return ParseServerResult::Err("location.front_pathが指定されていません");
			if (!(iss >> tmp_str) || tmp_str != "{")
				return ParseServerResult::Err("Config: locationブロックの開始が不正です");
			if (iss >> tmp_str)
				return ParseServerResult::Err("Config: locationの引数が多いです");
			ParseLocation(config_file, location);
			server.locations.push_back(location);
			if (config_file.eof())
				return ParseServerResult::Err("Config: サーバーブロックが終了していない");
		}
		else if (key == "location_back")
		{
			Location location;
			std::string tmp_str;
			if (!(iss >> location.back_path))
				return ParseServerResult::Err("location.back_pathが指定されていません");
			if (location.back_path[0] != '.')
				return ParseServerResult::Err("location.back_pathは拡張子で指定してください");
			if (!(iss >> tmp_str) || tmp_str != "{")
				return ParseServerResult::Err("Config: locationブロックの開始が不正です");
			if (iss >> tmp_str)
				return ParseServerResult::Err("Config: locationの引数が多いです");
			ParseLocation(config_file, location);
			server.locations.push_back(location);
			if (config_file.eof())
				return ParseServerResult::Err("Config: サーバーブロックが終了していない");
		}
		else if (key == "server_name")
		{
			Result<std::string, std::string> result = PullWord<std::string>(iss);
			server.name = result.unwrap();
		}
		else if (key == "listen")
		{
			Result<int, std::string> result = PullWord<int>(iss);
			server.port = result.unwrap();
		}
		else if (key == "root")
		{
			Result<std::string, std::string> result = PullWord<std::string>(iss);
			server.root = result.unwrap();
		}
		else if (key == "error_page")
		{
			ErrorPageMapResult errorPageMapResult = HandleErrorPageDirective(iss);
			if (!errorPageMapResult.ok())
				return ParseServerResult::Err(errorPageMapResult.unwrapErr());
			// TODO: errror_pageを複数設定できるようにする
			server.error_page = errorPageMapResult.unwrap();
		}
		else if (key == "client_max_body_size")
		{
			Result<size_t, std::string> result = PullWord<size_t>(iss);
			server.client_max_body_size = result.unwrap();
		}
		else if (key == "autoindex")
		{
			Result<std::string, std::string> result = PullWord<std::string>(iss);
			std::string autoindex_value = result.unwrap();
			server.autoindex = (autoindex_value == "on");
		}
		else if (key == "index")
		{
			Result<std::string, std::string> result = PullWord<std::string>(iss);
			server.index = result.unwrap();
		}
		else if (key == "}")
		{
			return (ParseServerResult::Ok(server));
		}
		else if (config_file.eof())
		{
			ParseServerResult::Err("Config: サーバーブロックが終了していない");
		}
		else if (key.empty() || key == "\t" || key == "\n")
		{
			continue;
		}
		else
		{
			ParseServerResult::Err("Config: 不正なディレクティブ");
		}
	}
	return ParseServerResult::Ok(server);
}

// 設定ファイルを解析するメインの関数
ParseResult ParseConfig(const char* config_path)
{
	std::ifstream config_file(config_path);
	std::string line;
	std::vector<Server> servers;

	if (!config_file.is_open())
		return ParseResult::Err("Failed to open file: " + std::string(config_path));
	while (std::getline(config_file, line))
	{
		std::istringstream iss(line);
		std::string key;
		iss >> key;

		if (key == "server")
		{
			std::string tmp_str;
			if (!(iss >> tmp_str) || tmp_str != "{")
				return ParseResult::Err("Config: serverブロックの開始が不正です");
			if (iss >> tmp_str)
				return ParseResult::Err("Config: serverの引数が多いです");
			ParseServerResult serverResult = ParseServer(config_file);
			if (!serverResult.ok())
				return ParseResult::Err(serverResult.unwrapErr());
			servers.push_back(serverResult.unwrap());
		}
	}
	return ParseResult::Ok(servers);
}
