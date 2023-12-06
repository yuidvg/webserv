#include "socket.hpp"

int initializeSocket(void)
{
	struct sockaddr_in6 addr;
	// socketの設定
	int server_fd = socket(AF_INET6,SOCK_STREAM,0);
	if (server_fd == -1)
	{
		std::cerr << "socket error" << std::endl;
		return (-1);
	}

	//  // Bind the socket
	// memset(&addr, 0, sizeof(addr));
	// addr.sin6_family = AF_INET6;
	// memcpy(&addr.sin6_addr, &in6addr_any, sizeof(in6addr_any));
	// addr.sin6_port = htons(SERVER_PORT);



	return (server_fd);
}