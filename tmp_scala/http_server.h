#include <sys/socket.h>
#include <netinet/in.h>	
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define SERVER_ADDR "127.0.0.1"
#define SERVER_PORT 8080
#define SIZE (5*1024)

/*
    http serverの設定
*/
int httpServer(int);
int recvRequestMessage(int, char*, unsigned int);
int parseRequestMessage(char*, char*, char*);
int getProcessing(char*, char*);
int createResponseMessage(char*, int, char*, char*, unsigned int);
int sendResponseMessage(int, char*, unsigned int);
unsigned int getFileSize(const char*);


/*
    wrap関数
*/
int wrap_socket(void);
int wrap_bind(int w_addr, struct sockaddr_in a_addr);
int wrap_listen(int w_addr, int backlog);
int wrap_accept(int w_addr, struct sockaddr *addr, socklen_t *addrlen);