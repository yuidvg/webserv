#include "http_server.h"

int wrap_socket()
{
	int w_addr = socket(AF_INET, SOCK_STREAM, 0);
    if (w_addr == -1) {
        printf("socket error\n");
		exit(1);
	}
	return (w_addr);
}

int wrap_bind(int w_addr,struct sockaddr_in a_addr)
{
	int res = bind(w_addr, (const struct sockaddr *)&a_addr, sizeof(a_addr));
	if (res == -1)
	{
		printf("bind error\n");
		close(w_addr);
		exit(1);
	}
	return (res);
}

int wrap_listen(int w_addr, int backlog)
{
	int res = listen(w_addr, backlog);
	if (res == -1)
	{
		printf("listen error\n");
		close(w_addr);
		exit(1);
	}
	return (res);
}

int wrap_accept(int w_addr, struct sockaddr *addr, socklen_t *addrlen)
{
	int res = accept(w_addr, NULL, NULL);
	if (res == -1)
	{
		printf("accept error\n");
        exit(1);
	}
	return (res);
}