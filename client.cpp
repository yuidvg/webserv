#include "srcs/server/socket.hpp"

int main()
{
	int sock = 0;
	struct sockaddr_in serv_addr;

	// ソケットの作成
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		std::cerr << "Socket creation error" << std::endl;
		return -1;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(80);

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
	}
	// ソケットのクローズ
	close(sock);

	return 0;
}
