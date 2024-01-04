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
#include "../Result/Result.hpp"

#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define YELLO "\x1b[33m"
#define NORMAL "\x1b[0m"

#define FRONT 0
#define BACK 1

/*
 ディレクティブの名前はconfigファイルに準拠して実装を行っているため、
 ルーティングの名前も同様にnginxに合わせLocationとする。
 */
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
	std::vector<std::string> cgi_executor;			 // CGIとして実行するプログラム
	std::string upload_path;			 // アップロードパス
	std::map<int, std::string> redirect; // リダイレクト先のURL
	Location()
	{
		front_path = "";
		back_path = "";
		root = "";
		autoindex = false;
		index = "index.html";
		client_max_body_size = 0;
		error_page.clear();
		allow_method.clear();
		cgi_executor.clear();
		upload_path = "";
		redirect.clear();
	}
};

// サーバーコンテキストの設定
struct Server
{
	std::string name;	   					// サーバー名
	size_t port;							 // ポート番号
	std::vector<Location> locations;	   // ロケーションの設定
	std::string root;					   // サーバー全体のルートディレクトリ
	std::map<int, std::string> error_page; // エラーページの設定
	size_t client_max_body_size;		   // サーバー全体のリクエストボディ最大サイズ
	bool autoindex;						   // ディレクトリリスティングの有効/無効
	std::string index;					   // デフォルトファイル名
	Server()
	{
		name = "";
		port = 80;
		root = "";
		error_page.clear();
		client_max_body_size = 1048576; // 1MB
		autoindex = false;
		index = "index.html";
		locations.clear();
	}
};

typedef Result<std::vector<Server>, std::string> ParseResult;
typedef Result<Server, std::string> ParseServerResult;
typedef Result<std::string, std::string> ParseRoutesResult;
typedef Result<std::map<int, std::string>, std::string> ErrorPageMapResult;


template <typename T>
Result<T, std::string> PullWord(std::istringstream& iss);
ParseServerResult ParseServer(std::ifstream& config_file);
ParseRoutesResult ParseLocation(std::ifstream& config_file, Location& location);
ErrorPageMapResult HandleErrorPageDirective(std::istringstream& iss);
void HandleLocationDirective(std::istringstream& iss, std::ifstream& config_file, Server& server, int type);

ParseResult ParseConfig(const char* config_path);

#endif
