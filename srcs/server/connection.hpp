#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include "socket.hpp"
#include "../config/parseConfig.hpp"
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
#include <algorithm>

#define TRUE 1
#define FALSE 0
#define RED "\x1b[31m"
#define YELLOW "\x1b[33m"
#define NORMAL "\x1b[0m"

typedef Result<int, std::string> NewSDResult;
typedef Result<Socket, std::string> FindConnectedVirtualServerResult;

class Connection
{
private:
    void AllCloseConnection();
    void CloseConnection(int fd); // 接続を閉じる
    FindConnectedVirtualServerResult FindConnectedVirtualServer(int sd, std::vector<Socket>& sockets);
    NewSDResult AcceptNewConnection(int listen_sd);               // 新規接続を受け入れる
    void ProcessConnection(int sd, Socket& socket); // 接続が確立されたソケットと通信する
    std::vector<int> listen_sockets;                                     // リスニングソケット
    std::vector<Socket> sockets;                                         // リスニングソケット
    int max_sd;                                                          // 最大のファイルディスクリプタ
    fd_set master_set;                                                   // ファイルディスクリプタの集合

    std::map<int, Socket> conn_socks; // connected_sockets<sd,socket> この仮想サーバーが受け持つソケットのリスト
public:
    Connection();  // コンストラクタ
    ~Connection(); // デストラクタ
    void deleteConnSock(int sd);
    void Start(std::vector<Server> servers); // サーバーを開始するためのメソッド
};

#endif