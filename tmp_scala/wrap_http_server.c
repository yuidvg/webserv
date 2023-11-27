#include "http_server.h"

ssize_t wrap_recv(int sock, void *request_message, size_t len, int flags)
{
	ssize_t res = recv(sock, request_message, SIZE, 0);
	if (res == -1)
	{
        /* 受信サイズが0の場合は相手が接続閉じていると判断 */
        printf("connection ended\n");
		exit(1);
	}
	return (res);
}