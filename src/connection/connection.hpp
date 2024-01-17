#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include "../config/parseConfig.hpp"
#include "../httpRequest/HttpRequest.hpp"
#include "../httpRequest/parseRequest.hpp"
#include "../httpResponse/build.hpp"
#include "eventLoop.hpp"
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


void allcloseConnection(fd_set &masterSet, Sockets &socketsIn, std::map<int, Socket> &connSockets);
void closeConnection(int fd, fd_set &masterSet, std::map<int, Socket> &connSockets); // 接続を閉じる
NewSocketResult newConnectedSocket(int listenSd, int &maxSd, fd_set &masterSet);     // 新規接続を受け入れる
bool processConnection(const Socket &socket); // 接続が確立されたソケットと通信する
void deleteConnSock(int sd, std::map<int, Socket> &connSocks);
void StartConnection(const std::vector<Server> servers); // サーバーを開始するためのメソッド
unsigned int getMaxSd(const Sockets socketsIn, const std::map<int, Socket> &connSocks);
void startConnection(const Servers servers);

#endif
