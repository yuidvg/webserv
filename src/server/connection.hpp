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

typedef utils::Result<int, std::string> NewSDResult;
typedef utils::Result<Socket, std::string> FindConnectedVirtualServerResult;

void allcloseConnection(int &maxSd, fd_set &masterSet, std::map<int, Socket> &connSocks);
void closeConnection(int fd, int &maxSd, fd_set &masterSet, std::map<int, Socket> &connSocks); // 接続を閉じる
NewSDResult acceptNewConnection(int listenSd, int &maxSd, fd_set &masterSet); // 新規接続を受け入れる
void processConnection(int sd, int &maxSd, Socket &socket,
                       std::map<int, Socket> &connSocks); // 接続が確立されたソケットと通信する
// std::vector<int> listenSockets;                 // リスニングソケット
// std::vector<Socket> sockets;                    // リスニングソケット
// int maxSd;                                      // 最大のファイルディスクリプタ
// fd_set masterSet;                               // ファイルディスクリプタの集合
// std::map<int, Socket> connSocks; // connectedsockets<sd,socket> この仮想サーバーが受け持つソケットのリスト
void deleteConnSock(int sd, std::map<int, Socket> &connSocks);
void StartConnection(const std::vector<Server> servers); // サーバーを開始するためのメソッド

#endif
