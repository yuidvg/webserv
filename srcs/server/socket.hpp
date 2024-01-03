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
#include "../config/parse_config.hpp"

#define TRUE 1
#define FALSE 0
#define RED "\x1b[31m"
#define NORMAL "\x1b[0m"


class Server {
private:
    void AcceptNewConnection(); // 新規接続を受け入れる
    void ProcessConnection(int socket); //接続が確立されたソケットと通信する
    void InitializeSocket(ConfigServer config_server);
    void CloseConnection(int fd);  // 接続を閉じる

    int listen_sd; // リスニングソケット
    int max_sd;    // 最大のファイルディスクリプタ
    fd_set master_set; // ファイルディスクリプタの集合
public:
    Server();  // コンストラクタ
    ~Server(); // デストラクタ

    int Start(ConfigServer config_server); // サーバーを開始するためのメソッド
};

#endif
