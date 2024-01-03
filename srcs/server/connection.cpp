#include "connection.hpp"

Connection::Connection()
{
}

Connection::~Connection()
{
}

void Connection::CloseConnection(int socket)
{
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
			CloseConnection(i);
		}
	}
}

InitializeResult Connection::InitializeSocket(int port)
{
	int sd = socket(PF_INET, SOCK_STREAM, 0);
	if (sd < 0)
		return(SocketResult::Err("socket() failed"));

	int flags = fcntl(sd, F_GETFL, 0);
	if (flags < 0)
	{
		close(sd);
		return(SocketResult::Err("fcntl() failed"));
	}
	flags |= O_NONBLOCK;
	if (fcntl(sd, F_SETFL, flags) < 0)
	{
		close(sd);
		return(SocketResult::Err("fcntl() failed to set non-blocking"));
	}

	struct sockaddr_in addr = {};
	addr.sin_family = PF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY); // IPv4アドレスを指定
	addr.sin_port = htons(port);			  // ポート番号を設定

	if (bind(sd, (struct sockaddr*)&addr, sizeof(addr)) < 0)
	{
		close(sd);
		return(SocketResult::Err("bind() failed"));
	}

	if (listen(sd, 5) < 0)
	{
		close(sd);
		return(SocketResult::Err("listen() failed"));
	}

	FD_ZERO(&master_set);
	return SocketResult::Ok(sd);
}

SocketResult Connection::AcceptNewConnection()
{
	int new_sd = accept(listen_sd, NULL, NULL);
	if (new_sd < 0)
	{
		// ノンブロッキング操作がブロックされた
		return (SocketResult::Err("accept() failed"));
	}
	else
	{
		FD_SET(new_sd, &master_set);
		if (new_sd > max_sd)
		{
			max_sd = new_sd;
		}
	}
	return SocketResult::Ok(0);
}

// 接続が確立されたソケットと通信する
void Connection::ProcessConnection(int socket)
{
	bool close_conn = false;
	char buffer[80];
	int rc;

	while (true)
	{
		rc = recv(socket, buffer, sizeof(buffer), 0);
		if (rc < 0)
		{
			std::cerr << "recv() failed: " << std::endl;
			close_conn = true;
			break;
		}

		if (rc == 0)
		{
			std::cout << "Connection closed" << std::endl;
			close_conn = true;
			break;
		}

		int len = rc;
		std::cout << "Received " << len << " bytes: " << buffer << std::endl;

		// TODO: 受け取ったHTTPリクエストを解析する
		// HTTPリクエスト解析のロジックをここに実装

		// TODO: HTTPレスポンスを作成する
		// HTTPレスポンス作成のロジックをここに実装
		std::string response = "HTTP/1.1 200 OK\r\nContent-Length: 22\r\n\r\n<h1>Hello Webserv</h1>";
		rc = send(socket, response.c_str(), response.length(), 0);
		if (rc < 0)
		{
			std::cerr << "send() failed: " << std::endl;
			close_conn = true;
			break;
		}
	}

	if (close_conn)
	{
		CloseConnection(socket);
	}
}

int Connection::GetListenSocket() const
{
	return listen_sd;
}

void Connection::Start(std::vector<Server> servers)
{
	// 各仮想サーバーのソケットを初期化し、監視セットに追加
	// for (size_t i = 0; i < servers.size(); ++i)
	// {
		// InitializeResult resultSd = InitializeSocket(servers[i].port);
	InitializeResult resultSd = InitializeSocket(servers[0].port);
	if (!resultSd.ok())
	{
		std::cerr << resultSd.unwrapErr() << std::endl;
		return;
	}
	this->listen_sd = resultSd.unwrap();
	std::cout << "listen_sd = " << this->listen_sd << std::endl;
	FD_SET(listen_sd, &master_set);
	if (listen_sd > max_sd)
	{
		max_sd = listen_sd;
	}
	// }

	fd_set working_set;
	struct timeval timeout;
	int end_server = FALSE;

	while (end_server == FALSE)
	{
		memcpy(&working_set, &master_set, sizeof(master_set));
		timeout.tv_sec = 3 * 60; // タイムアウト値を3分に設定
		timeout.tv_usec = 0;

		std::cout << "Waiting on select()..." << std::endl;
		int rc = select(max_sd + 1, &working_set, NULL, NULL, &timeout);
		if (rc < 0)
		{
			std::cerr << "select() failed: " << std::endl;
			break;
		}
		else if (rc == 0)
		{
			std::cout << "select() timed out. End program." << std::endl;
			break;
		}

		for (int i = 0; i <= max_sd; ++i)
		{
			// working_setの集合に特定のFD(i)が存在するかどうかを判別する
			if (FD_ISSET(i, &working_set))
			{
				if (i == listen_sd)
				{
					SocketResult socketResult = AcceptNewConnection();
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
	return;
}
