#include "srcs/server/connection.hpp"
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>

bool is_digits_only(const char* str) {
	while (*str) {
		if (!isdigit((unsigned char)*str))
			return false;
		str++;
	}
	return true;
}

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		std::cerr << "ポート番号を指定してください" << std::endl;
		return -1;
	}
	int sock = 0;
	struct sockaddr_in serv_addr;
	// ソケットの作成
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		std::cerr << "Socket creation error" << std::endl;
		return -1;
	}

	serv_addr.sin_family = AF_INET;
	if (!is_digits_only(argv[1]))
	{
		std::cerr << "Invalid port number" << std::endl;
		return -1;
	}
	int port = atoi(argv[1]);
	serv_addr.sin_port = htons(port);

	// IPv4アドレスへの変換
	if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
	{
		std::cerr << "Invalid address / Address not supported" << std::endl;
		return -1;
	}

	// サーバーへの接続
	if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
	{
		std::cerr << "Connection failed" << std::endl;
		return -1;
	}

	while (TRUE)
	{
		// メッセージの送信
		std::string message = "Hello Server!";
		send(sock, message.c_str(), message.length(), 0);
		std::cout << "Message sent" << std::endl;

		// レスポンスの受信
		char buffer[1024] = { 0 };
		int valread = read(sock, buffer, 1024);
		if (valread < 0)
		{
			std::cerr << "Read error" << std::endl;
			return -1;
		}
		std::cout << "Server: " << buffer << std::endl;
		usleep(3000000);
	}
	// ソケットのクローズ
	close(sock);

	return 0;
}
