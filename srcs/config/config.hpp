#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <iostream>
#include <map>
#include <string>
#include <vector>

/*
 ディレクティブの名前はconfigファイルに準拠して実装を行っているため、
 ルーティングの名前も同様にnginxに合わせLocationとする。
 */
// ルートコンテキストの設定
struct Location
{
    std::string front_path;                // location(前方一致)で指定されたパス
    std::string back_path;                 // location(後方一致)で指定されたパス
    std::string root;                      // ルートディレクトリ
    bool autoindex;                        // ディレクトリリスティングの有効/無効
    std::string index;                     // デフォルトファイル名
    size_t client_max_body_size;           // リクエストボディの最大サイズ
    std::map<int, std::string> error_page; // エラーページの設定
    std::vector<std::string> allow_method; // 許可されるHTTPメソッド（GET, POST, DELETE）
    std::vector<std::string> cgi_executor; // CGIとして実行するプログラム
    std::string upload_path;               // アップロードパス
    std::map<int, std::string> redirect;   // リダイレクト先のURL
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
    std::string name;                      // サーバー名
    size_t port;                           // ポート番号
    std::vector<Location> locations;       // ロケーションの設定
    std::string root;                      // サーバー全体のルートディレクトリ
    std::map<int, std::string> error_page; // エラーページの設定
    size_t client_max_body_size;           // サーバー全体のリクエストボディ最大サイズ
    bool autoindex;                        // ディレクトリリスティングの有効/無効
    std::string index;                     // デフォルトファイル名
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

#endif
