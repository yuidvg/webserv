#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <iostream>
#include <map>
#include <string>
#include <vector>

struct Location
{
    const std::string path;         // location(前方一致)で指定されたパス
    const std::string root;         // ルートディレクトリ
    const bool autoindex;           // ディレクトリリスティングの有効/無効
    const std::string index;        // デフォルトファイル名
    const size_t clientMaxBodySize; // リクエストボディの最大サイズ
    const std::map<int, std::string> errorPages;
    const std::vector<std::string> allowMethods; // 許可されるHTTPメソッド（GET, POST, DELETE）
    const std::string cgiExtension;
    const std::string uploadPath;
    const std::map<int, std::string> redirect; // リダイレクト先のURL

    Location();
    Location(const std::string path, const std::string root, const bool autoindex, const std::string index,
             const size_t clientMaxBodySize, const std::map<int, std::string> errorPages,
             const std::vector<std::string> allowMethods, const std::string cgiExtension, const std::string uploadPath,
             const std::map<int, std::string> redirect)
        : path(path), root(root), autoindex(autoindex), index(index), clientMaxBodySize(clientMaxBodySize),
          errorPages(errorPages), allowMethods(allowMethods), cgiExtension(cgiExtension), uploadPath(uploadPath),
          redirect(redirect)
    {
    }
};

struct Server
{
    const std::string name; // サーバー名
    const size_t port;
    const std::string root; // サーバー全体のルートディレクトリ
    const std::map<int, std::string> errorPages;
    const size_t clientMaxBodySize;        // リクエストボディ最大サイズ
    const bool autoindex;                  // ディレクトリリスティングの有効/無効
    const std::string index;               // デフォルトファイル名
    const std::vector<Location> locations; // ロケーションの設定
    Server();
    Server(const std::string name, const size_t port, const std::string root,
           const std::map<int, std::string> errorPages, const size_t clientMaxBodySize, const bool autoindex,
           const std::string index, const std::vector<Location> locations)
        : name(name), port(port), root(root), errorPages(errorPages), clientMaxBodySize(clientMaxBodySize),
          autoindex(autoindex), index(index), locations(locations)
    {
    }
};

#endif
