#ifndef PARSE_CONFIG_HPP
#define PARSE_CONFIG_HPP

#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define YELLO "\x1b[33m"
#define NORMAL "\x1b[0m"

#define FRONT 0
#define BACK 1

// ルートコンテキストの設定
struct Location
{
	std::string front_path;				   // location(前方一致)で指定されたパス
	std::string back_path;				   // location(後方一致)で指定されたパス
	std::string root;					   // ルートディレクトリ
	bool autoindex;						   // ディレクトリリスティングの有効/無効
	std::string index;					   // デフォルトファイル名
	size_t client_max_body_size;		   // リクエストボディの最大サイズ
	std::map<int, std::string> error_page; // エラーページの設定
	std::vector<std::string> allow_method; // 許可されるHTTPメソッド（GET, POST, DELETE）
	std::string cgi_executor;			   // CGIとして実行する拡張子
	std::string upload_path;			   // アップロードパス
	std::map<int, std::string> redirect;   // リダイレクト先のURL
	// 初期化メソッド
	void Initialize()
	{
		front_path = "";
		back_path = "";
		root = "";
		autoindex = false;
		index = "index.html";
		client_max_body_size = 0;
		error_page.clear();
		allow_method.clear();
		cgi_executor = "";
		upload_path = "";
		redirect.clear();
	}
};

// サーバーコンテキストの設定
struct Server
{
	std::string server_name;			   // サーバー名
	size_t port;						   // ポート番号
	std::vector<Location> locations;	   // ロケーションの設定
	std::string root;					   // サーバー全体のルートディレクトリ
	std::map<int, std::string> error_page; // エラーページの設定
	size_t client_max_body_size;		   // サーバー全体のリクエストボディ最大サイズ
	bool autoindex;						   // ディレクトリリスティングの有効/無効
	std::string index;					   // デフォルトファイル名
	// 初期化メソッド
	void InitializeServer()
	{
		server_name = "";
		port = 80;
		root = "";
		error_page.clear();
		client_max_body_size = 1048576; // 1MB
		autoindex = false;
		index = "index.html";
		locations.clear();
	}
};

class Config
{
private:
	Config();
	template <typename T>
	T PullWord(std::istringstream& iss);
	void ParseServer(std::ifstream& config_file, Server& server);
	void ParseLocation(std::ifstream& config_file, Location& location);
	// Directive
	// Server Location
	void HandleErrorPageDirective(std::istringstream& iss, std::map<int, std::string>& error_page);
	// Location
	void HandleAllowMethodDirective(std::istringstream& iss, std::vector<std::string>& allow_method);
	void HandleRedirectDirective(std::istringstream& iss, std::map<int, std::string>& redirect);
	// Server
	void HandleLocationDirective(std::istringstream& iss, std::ifstream& config_file, Server& server, int type);

	void ParseConfig(const char* config_path);

public:
	Config(const char* config_path);
	~Config();
	void DebugPrint(void) const;
	void PrintLocation(const Location& location) const;
	void PrintServer(const Server& server) const;
	// 複数のサーバーを管理する
	std::vector<Server> servers;
};

#endif
