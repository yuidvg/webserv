#include "connection.hpp"
#include "../HTTPRequest/HTTPParser.hpp"

Connection::Connection()
{
}

Connection::~Connection()
{
}

void Connection::CloseConnection(int sd)
{
	std::cout << YELLOW << "close sd: " << NORMAL << sd << std::endl;

	deleteConnSock(sd);
	close(sd);
	FD_CLR(sd, &master_set);
	if (sd == max_sd)
	{
		while (max_sd > 0 && !FD_ISSET(max_sd, &master_set))
		{
			max_sd -= 1;
		}
		if (max_sd == 0 && !FD_ISSET(0, &master_set))
		{
			// 有効なディスクリプタが残っていない
			max_sd = -1;
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

NewSDResult Connection::AcceptNewConnection(const int listen_sd)
{
	int new_sd = -1;
	new_sd = accept(listen_sd, NULL, NULL);
	if (new_sd < 0)
	{
		if (errno != EWOULDBLOCK)
		{
			// エラーメッセージを返す
			std::string error_msg = "accept() failed on socket ";
			return NewSDResult::Err(error_msg);
		}
	}
	std::cout << GREEN << "New incoming connection " << new_sd << NORMAL << std::endl;
	FD_SET(new_sd, &master_set);
	if (new_sd > max_sd)
	{
		max_sd = new_sd;
	}
	// 新しい接続を受け入れた
	return NewSDResult::Ok(new_sd);
}

void Connection::deleteConnSock(int sd)
{
	// c++98で使用可能 https://en.cppreference.com/w/cpp/container/map/erase
	conn_socks.erase(sd);
}

// 接続が確立されたソケットと通信する
void Connection::ProcessConnection(int sd, Socket &socket)
{
	char buffer[500000] = { 0 }; // Initialize buffer with null
	int rc;

	// Debug用
	std::cout << GREEN;
	std::cout << "Port = " << socket.getServer().port << std::endl;
	std::cout << "server_name = " << socket.getServer().name << std::endl;
	std::cout << NORMAL;

	rc = recv(sd, buffer, sizeof(buffer) - 1, 0);
	if (rc < 0)
	{
		std::cerr << "recv() failed " << strerror(errno) << std::endl;
		CloseConnection(sd);
		return;
	}
	else if (rc == 0)
	{
		std::cout << "Connection closed" << std::endl;
		CloseConnection(sd);
		return;
	}
	std::cout << "Received \n"
			  << GREEN << rc << " bytes: " << buffer << NORMAL << std::endl;

	// TODO: 受け取ったHTTPリクエストを解析する
	std::istringstream	buf(buffer);
	HTTPParseResult	parserResult = parseHTTPRequest(buf, socket.getServer());
	if (!parserResult.ok())
	{
		std::cerr << RED << "error_code: " << parserResult.unwrapErr() << NORMAL << std::endl;
		CloseConnection(sd);
		return;
	}

	// TODO: HTTPレスポンスを作成する
	// HTTPレスポンス作成のロジックをここに実装
	std::string response = "HTTP/1.1 200 OK\r\nContent-Length: 22\r\n\r\n<h1>Hello Webserv</h1>";
	rc = send(sd, response.c_str(), response.length(), 0);
	if (rc < 0)
	{
		std::cerr << "send() failed: " << std::endl;
		CloseConnection(sd);
		return;
	}
}

void Connection::Start(std::vector<Server> servers)
{
	// 各仮想サーバーのソケットを初期化し、監視セットに追加
	FD_ZERO(&master_set);
	for (size_t i = 0; i < servers.size(); ++i)
	{
		Socket socket(servers[i]);
		sockets.push_back(socket); // 新しいリスニングソケットを追加
		int listen_sd = socket.getListenSocket();
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
	timeout.tv_sec = 30;
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
					NewSDResult newSDResult = AcceptNewConnection(i);
					if (!newSDResult.ok())
					{
						std::cerr << newSDResult.unwrapErr() << std::endl;
						AllCloseConnection();
						return;
					}
					for (size_t index = 0; index < sockets.size(); ++index)
					{
						if (sockets[index].getListenSocket() == i)
						{
							std::cout << "新しい接続を受け入れた" << std::endl;
							conn_socks.insert(std::make_pair(newSDResult.unwrap(), sockets[index]));
						}
					}
				}
				else
				{
					ProcessConnection(i, conn_socks[i]);
				}


				// Debug用
				for (std::vector<int>::iterator it = listen_sockets.begin(); it != listen_sockets.end(); ++it)
				{
					std::cout << *it << " :Listen socket" << std::endl;
				}
				for (std::map<int, Socket>::iterator it = conn_socks.begin(); it != conn_socks.end(); ++it)
				{
					std::cout << it->first << " :" << it->second.getServer().name << std::endl;
				}
			}
		}
	}
	std::cout << "Closing socket discriptor..." << std::endl;
	AllCloseConnection();
}
