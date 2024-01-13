#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include "../config/parseConfig.hpp"
#include "../httpRequest/parseRequest.hpp"
#include "../httpResponse/buildResponse.hpp"
#include "../utils/utils.hpp"
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

typedef Result<int, std::string> NewSDResult;
typedef Result<Socket, std::string> FindConnectedVirtualServerResult;

class Connection
{
  private:
    void AllCloseConnection();
    void CloseConnection(int fd); // 接続を閉じる
    FindConnectedVirtualServerResult FindConnectedVirtualServer(int sd, std::vector<Socket> &sockets);
    NewSDResult AcceptNewConnection(int listenSd);  // 新規接続を受け入れる
    void ProcessConnection(int sd, Socket &socket); // 接続が確立されたソケットと通信する
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
