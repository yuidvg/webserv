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


// ルートコンテキストの設定
struct Location
{
	std::string path;                      // locationで指定されたパス
	std::string root;                      // ルートディレクトリ
	bool autoindex;                        // ディレクトリリスティングの有効/無効
	std::vector<std::string> index;        // デフォルトファイル名
	size_t client_max_body_size;           // リクエストボディの最大サイズ
	std::map<int, std::string> error_page; // エラーページの設定
	std::vector<std::string> allow_method; // 許可されるHTTPメソッド（GET, POST, DELETE）
	std::string cgi_path;                  // CGIスクリプトのパス
	std::string upload_path;               // アップロードパス
	std::string redirect;                  // リダイレクト先のURL
};

// サーバーコンテキストの設定
struct Server
{
	std::string server_name;               // [server_name]サーバー名
	std::string host;                      // [server_name]ホスト名
	int ip_addr;                           // [server_name]IPアドレス
	size_t port;                           // ポート番号
	std::vector<Location> locations;       // ロケーションの設定
	std::string root;                      // サーバー全体のルートディレクトリ
	std::map<int, std::string> error_page; // エラーページの設定
	size_t client_max_body_size;           // サーバー全体のリクエストボディ最大サイズ
	bool autoindex;                        // ディレクトリリスティングの有効/無効
	std::vector<std::string> index;        // デフォルトファイル名
};

// 複数のサーバーを管理する
struct WebServer
{
	std::vector<Server> servers;
};

class Config
{
private:
	Config();
	template <typename T>
	T PullWord(std::istringstream& iss, int num);
	std::string PullWord(std::istringstream& iss, int num);
	void ParseServer(std::ifstream& config_file, Server& server);
	void ParseLocation(std::ifstream& config_file, Location& location);
	void ParseConfig(const std::string& config_path);
	void InitializeServer(Server& server);
	void InitializeLocation(Location& location);

public:
	Config(const std::string& config_path);
	~Config();
	void PrintLocation(const Location& location);
	void PrintServer(const Server& server);
	WebServer webserver;
};

#endif
