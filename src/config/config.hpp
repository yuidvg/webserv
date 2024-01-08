#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <iostream>
#include <map>
#include <string>
#include <vector>

struct Location
{
    std::string path;                      // location(前方一致)で指定されたパス
    std::string root;                      // ルートディレクトリ
    bool autoindex;                        // ディレクトリリスティングの有効/無効
    std::string index;                     // デフォルトファイル名
    size_t clientMaxBodySize;              // リクエストボディの最大サイズ
    std::map<int, std::string> errorPages;
    std::vector<std::string> allowMethods;  // 許可されるHTTPメソッド（GET, POST, DELETE）
    std::string cgiExtension;
    std::string uploadPath;
    std::map<int, std::string> redirect; // リダイレクト先のURL
    Location()
    {
        path = "";
        root = "";
        autoindex = false;
        index = "index.html";
        clientMaxBodySize = 0;
        errorPages.clear();
        allowMethods.push_back("GET");
        allowMethods.push_back("POST");
        allowMethods.push_back("DELETE");
        cgiExtension.clear();
        uploadPath = "";
        redirect.clear();
    }
};

struct Server
{
    std::string name;                      // サーバー名
    size_t port;                           
    std::vector<Location> locations;       // ロケーションの設定
    std::string root;                      // サーバー全体のルートディレクトリ
    std::map<int, std::string> errorPages;
    size_t clientMaxBodySize;              // リクエストボディ最大サイズ
    bool autoindex;                        // ディレクトリリスティングの有効/無効
    std::string index;                     // デフォルトファイル名
    Server()
    {
        name = "";
        port = 80;
        root = "";
        errorPages.clear();
        clientMaxBodySize = 1048576; // 1MB
        autoindex = false;
        index = "index.html";
        locations.clear();
    }
};

#endif
