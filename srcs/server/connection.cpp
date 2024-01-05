#include "connection.hpp"

Connection::Connection()
{
}

Connection::~Connection()
{
}

void Connection::CloseConnection(int socket)
{
	std::cout << YELLO << "close socket: " << socket << NORMAL << std::endl;
	close(socket);
	FD_CLR(socket, &master_set);
	if (socket == max_sd)
	{
		while (!FD_ISSET(max_sd, &master_set))
		{
			max_sd -= 1;
		}
	}
}

void Connection::AllCloseConnection()
{
	for (int i = 0; i <= max_sd; ++i)
	{
		if (FD_ISSET(i, &master_set))
		{
			std::cout << "接続を閉じています " << i << std::endl;
			CloseConnection(i);
		}
	}
	std::cout << "すべての接続を閉じた" << std::endl;
}

SocketResult Connection::AcceptNewConnection(const int listen_sd)
{
	int new_sd = -1;
	while (true)
	{
		new_sd = accept(listen_sd, NULL, NULL);
		if (new_sd < 0)
		{
			if (errno != EWOULDBLOCK)
			{
				// エラーメッセージを返す
				std::string error_msg = "accept() failed on socket ";
				return SocketResult::Err(error_msg);
			}
			break;
		}
		std::cout << GREEN << "New incoming connection " << new_sd << NORMAL << std::endl;
		FD_SET(new_sd, &master_set);
		if (new_sd > max_sd)
		{
			max_sd = new_sd;
		}
	}
	// 新しい接続はない
	return SocketResult::Ok(0);
}

// 接続が確立されたソケットと通信する
void Connection::ProcessConnection(int socket)
{
	char buffer[80] = {0}; // Initialize buffer with null
	int rc;

	rc = recv(socket, buffer, sizeof(buffer) - 1, 0); // Leave space for null terminator
	if (rc < 0)
	{
		std::cerr << "recv() failed " << strerror(errno) << std::endl;
		CloseConnection(socket);
		return ;
	}
	else if (rc == 0)
	{
		std::cout << "Connection closed" << std::endl;
		CloseConnection(socket);
		return ;
	}
	else
	{
		std::cout << "Received " << rc << " bytes: " << buffer << std::endl;
	}

	// TODO: 受け取ったHTTPリクエストを解析する
	// HTTPリクエスト解析のロジックをここに実装

	// TODO: HTTPレスポンスを作成する
	// HTTPレスポンス作成のロジックをここに実装
	std::string response = "HTTP/1.1 200 OK\r\nContent-Length: 22\r\n\r\n<h1>Hello Webserv</h1>";
	rc = send(socket, response.c_str(), response.length(), 0);
	if (rc < 0)
	{
		std::cerr << "send() failed: " << std::endl;
		CloseConnection(socket);
		return ;
	}
}

void Connection::Start(std::vector<Server> servers)
{
	// 各仮想サーバーのソケットを初期化し、監視セットに追加
	FD_ZERO(&master_set);
	for (size_t i = 0; i < servers.size(); ++i)
	{
		Socket socket(servers[i].port);
		int listen_sd = socket.getSocket();
		listen_sockets.push_back(listen_sd); // 新しいリスニングソケットを追加
		FD_SET(listen_sd, &master_set);
		if (listen_sd > max_sd)
		{
			max_sd = listen_sd;
		}
	}

	fd_set working_set;
	struct timeval timeout;
	int end_server = FALSE;
	timeout.tv_sec = 1 * 60; // タイムアウト値を1分に設定
	timeout.tv_usec = 0;

	while (end_server == FALSE)
	{
		memcpy(&working_set, &master_set, sizeof(master_set));

		std::cout << "Waiting on select()..." << std::endl;
		int rc = select(max_sd + 1, &working_set, NULL, NULL, &timeout);
		if (rc < 0)
		{
			std::cerr << "select() failed: " << strerror(errno) << std::endl;
			break;
		}
		else if (rc == 0)
		{
			std::cout << "select() timed out. End program." << std::endl;
			break;
		}

		int desc_ready = rc;
		for (int i = 0; i <= max_sd && desc_ready > 0; ++i)
		{
			if (FD_ISSET(i, &working_set))
			{
				// 読み取り可能なfdが見つかったら、検索を停止するため。
				desc_ready -= 1;
				// リスニングソケットの確認
				if (std::find(listen_sockets.begin(), listen_sockets.end(), i) != listen_sockets.end())
				{
					SocketResult socketResult = AcceptNewConnection(i);
					if (!socketResult.ok())
					{
						std::cerr << socketResult.unwrapErr() << std::endl;
						AllCloseConnection();
						return;
					}
				}
				else
				{
					ProcessConnection(i);
				}
			}
		}
	}

	AllCloseConnection();
}
