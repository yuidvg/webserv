#ifndef PARSE_CONFIG_HPP
#define PARSE_CONFIG_HPP

#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define YELLO "\x1b[33m"
#define NORMAL "\x1b[0m"

// ルートコンテキストの設定
struct Route
{
    std::string path;                       // locationで指定されたパス
    std::string root;                       // ルートディレクトリ
    bool autoindex;                         // ディレクトリリスティングの有効/無効
    std::string index;                      // デフォルトファイル名
    size_t client_max_body_size;            // リクエストボディの最大サイズ
    std::map<int, std::string> error_page;  // エラーページの設定
    std::vector<std::string> allow_method;  // 許可されるHTTPメソッド（GET, POST, DELETE）
    std::vector<std::string> cgiExtensions; // CGIスクリプトの拡張子
    std::string cgi_path;                   // CGIスクリプトのパス
    std::string upload_path;                // アップロードパス
    std::map<int, std::string> redirect;    // リダイレクト先のURL
};

// サーバーコンテキストの設定
struct Server
{
    std::string server_name;               // サーバー名
    size_t port;                           // ポート番号
    std::vector<Route> routes;             // Routesの設定
    std::string root;                      // サーバー全体のルートディレクトリ
    std::map<int, std::string> error_page; // エラーページの設定
    size_t client_max_body_size;           // サーバー全体のリクエストボディ最大サイズ
    bool autoindex;                        // ディレクトリリスティングの有効/無効
    std::string index;                     // デフォルトファイル名
};

class Config
{
  private:
    Config();
    template <typename T> T PullWord(std::istringstream &iss);
    void ParseServer(std::ifstream &config_file, Server &server);
    void ParseLocation(std::ifstream &config_file, Route &location);
    void ParseConfig(const char *config_path);
    void InitializeServer(Server &server);
    void InitializeLocation(Route &location);

  public:
    Config(const char *config_path);
    ~Config();
    void PrintLocation(const Route &location);
    void PrintServer(const Server &server);
};

#endif
