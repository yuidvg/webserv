#include "connection.hpp"
#include "../HttpRequest/RequestParser.hpp"
#include "../utils/utils.hpp"

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
	FD_CLR(sd, &masterSet);
	if (sd == maxSd)
	{
		while (maxSd > 0 && !FD_ISSET(maxSd, &masterSet))
		{
			maxSd -= 1;
		}
		if (maxSd == 0 && !FD_ISSET(0, &masterSet))
		{
			// 有効なディスクリプタが残っていない
			maxSd = -1;
		}
	}
}

void Connection::AllCloseConnection()
{
	for (int i = 0; i <= maxSd; ++i)
	{
		if (FD_ISSET(i, &masterSet))
		{
			CloseConnection(i);
		}
	}
}

NewSDResult Connection::AcceptNewConnection(const int listenSd)
{
	int newSd = -1;
	newSd = accept(listenSd, NULL, NULL);
	if (newSd < 0)
	{
		if (errno != EWOULDBLOCK)
		{
			// エラーメッセージを返す
			std::string errorMsg = "accept() failed on socket ";
			return NewSDResult::Err(errorMsg);
		}
	}
	std::cout << GREEN << "New incoming connection " << newSd << NORMAL << std::endl;
	FD_SET(newSd, &masterSet);
	if (newSd > maxSd)
	{
		maxSd = newSd;
	}
	// 新しい接続を受け入れた
	return NewSDResult::Ok(newSd);
}

void Connection::deleteConnSock(int sd)
{
	// c++98で使用可能 https://en.cppreference.com/w/cpp/container/map/erase
	connSocks.erase(sd);
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
		utils::printError(std::string("recv() failed: " + std::string(strerror(errno))));
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

	std::istringstream	buf(buffer);
	HttpParseResult	parserResult = parseHttpRequest(buf, socket.getServer());
	if (!parserResult.ok())
	{
		utils::printError(std::string("parseHttpRequest() failed: " + utils::to_string(parserResult.unwrapErr())));
		CloseConnection(sd);
		return;
	}

	// TODO: HTTPレスポンスを作成する
	// HTTPレスポンス作成のロジックをここに実装
	std::string response = "HTTP/1.1 200 OK\r\nContent-Length: 22\r\n\r\n<h1>Hello Webserv</h1>";
	rc = send(sd, response.c_str(), response.length(), 0);
	if (rc < 0)
	{
		utils::printError("send() failed: ");
		CloseConnection(sd);
		return;
	}
}

void Connection::Start(const std::vector<Server> servers)
{
	// 各仮想サーバーのソケットを初期化し、監視セットに追加
	FD_ZERO(&masterSet);
	for (size_t i = 0; i < servers.size(); ++i)
	{
		Socket socket(servers[i]);
		sockets.push_back(socket); // 新しいリスニングソケットを追加
		int listenSd = socket.getListenSocket();
		listenSockets.push_back(listenSd); // 新しいリスニングソケットを追加
		FD_SET(listenSd, &masterSet);
		if (listenSd > maxSd)
		{
			maxSd = listenSd;
		}
	}

	fd_set workingSet;
	struct timeval timeout;
	int endServer = FALSE;
	timeout.tv_sec = 3 * 60;
	timeout.tv_usec = 0;

	while (endServer == FALSE)
	{
		memcpy(&workingSet, &masterSet, sizeof(masterSet));

		std::cout << "Waiting on select()..." << std::endl;
		int rc = select(maxSd + 1, &workingSet, NULL, NULL, &timeout);
		if (rc < 0)
		{
			utils::printError(std::string("select() failed: " + std::string(strerror(errno))));
			break;
		}
		else if (rc == 0)
		{
			std::cout << "select() timed out. End program." << std::endl;
			break;
		}

		int descReady = rc;
		for (int i = 0; i <= maxSd && descReady > 0; ++i)
		{
			if (FD_ISSET(i, &workingSet))
			{
				// 読み取り可能なfdが見つかったら、検索を停止するため。
				descReady -= 1;
				// リスニングソケットの確認
				if (std::find(listenSockets.begin(), listenSockets.end(), i) != listenSockets.end())
				{
					NewSDResult newSDResult = AcceptNewConnection(i);
					if (!newSDResult.ok())
					{
						utils::printError(newSDResult.unwrapErr());
						AllCloseConnection();
						return;
					}
					for (size_t index = 0; index < sockets.size(); ++index)
					{
						if (sockets[index].getListenSocket() == i)
						{
							std::cout << "新しい接続を受け入れた" << std::endl;
							connSocks.insert(std::make_pair(newSDResult.unwrap(), sockets[index]));
						}
					}
				}
				else
				{
					ProcessConnection(i, connSocks[i]);
				}


				// Debug用
				for (std::vector<int>::iterator it = listenSockets.begin(); it != listenSockets.end(); ++it)
				{
					std::cout << *it << " :Listen socket" << std::endl;
				}
				for (std::map<int, Socket>::iterator it = connSocks.begin(); it != connSocks.end(); ++it)
				{
					std::cout << it->first << " :" << it->second.getServer().name << std::endl;
				}
			}
		}
	}
	std::cout << "Closing socket discriptor..." << std::endl;
	AllCloseConnection();
}
