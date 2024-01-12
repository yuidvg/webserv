#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include "../config/parseConfig.hpp"
#include "socket.hpp"
#include <algorithm>
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

#define TRUE 1
#define FALSE 0
#define RED "\x1b[31m"
#define YELLOW "\x1b[33m"
#define NORMAL "\x1b[0m"

typedef utils::Result<int, std::string> NewSDResult;
typedef utils::Result<Socket, std::string> FindConnectedVirtualServerResult;

class Connection
{
  private:
    void allcloseConnection();
    void closeConnection(int fd); // 接続を閉じる
    FindConnectedVirtualServerResult findConnectedVirtualServer(int sd, std::vector<Socket> &sockets);
    NewSDResult acceptNewConnection(int listenSd);  // 新規接続を受け入れる
    void processConnection(int sd, Socket &socket); // 接続が確立されたソケットと通信する
    std::vector<int> listenSockets;                 // リスニングソケット
    std::vector<Socket> sockets;                    // リスニングソケット
    int maxSd;                                      // 最大のファイルディスクリプタ
    fd_set masterSet;                               // ファイルディスクリプタの集合
    std::map<int, Socket> connSocks; // connectedsockets<sd,socket> この仮想サーバーが受け持つソケットのリスト
  public:
    Connection();  // コンストラクタ
    ~Connection(); // デストラクタ
    void deleteConnSock(int sd);
    void Start(const std::vector<Server> servers); // サーバーを開始するためのメソッド
};

#endif
