#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include "../config/parseConfig.hpp"
#include "../httpRequest/HttpRequest.hpp"
#include "../httpRequest/parseRequest.hpp"
#include "../httpResponse/build.hpp"
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

typedef Result<const int, const std::string> NewSDResult;

void allcloseConnection(const int &maxSd, const fd_set &masterSet, const std::map<int, Socket> &connSocks);
void closeConnection(const int fd, const int &maxSd, const fd_set &masterSet,
                     const std::map<int, Socket> &connSocks);                                   // 接続を閉じる
NewSDResult acceptNewConnection(const int listenSd, const int &maxSd, const fd_set &masterSet); // 新規接続を受け入れる
void processConnection(const int sd, const int &maxSd,
                       const std::map<int, Socket> &connSocks); // 接続が確立されたソケットと通信する
void deleteConnSock(const int sd, const std::map<int, Socket> &connSocks);
void StartConnection(const std::vector<Server> servers); // サーバーを開始するためのメソッド

#endif
