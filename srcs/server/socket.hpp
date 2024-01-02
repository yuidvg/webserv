#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <arpa/inet.h>
#include <cstring>
#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>

#define SERVER_PORT 12345

#define TRUE 1
#define FALSE 0
#define RED "\x1b[31m"
#define NORMAL "\x1b[0m"


class Server {
    // private:
    //     void SetupSocket();            // ソケットのセットアップ
    //     void ProcessConnections();     // 接続の処理
    //     void HandleNewConnection();    // 新規接続の処理
    //     void HandleExistingConnection(int fd); // 既存の接続の処理
    //     void CloseConnection(int fd);  // 接続を閉じる

    //     int listen_sd; // リスニングソケット
    //     int max_sd;    // 最大のファイルディスクリプタ
    //     fd_set master_set; // ファイルディスクリプタセット
public:
    Server();  // コンストラクタ
    ~Server(); // デストラクタ

    int Start(); // サーバーを開始するためのメソッド
};

#endif
