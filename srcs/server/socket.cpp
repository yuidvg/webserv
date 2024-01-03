#include "socket.hpp"

Server::Server() {

}

Server::~Server() {

}


void Server::CloseConnection(int socket) {
	close(socket);
	FD_CLR(socket, &master_set);
	if (socket == max_sd) {
		while (!FD_ISSET(max_sd, &master_set)) {
			max_sd -= 1;
		}
	}
}

void Server::InitializeSocket(ConfigServer config_server) {
	listen_sd = socket(PF_INET, SOCK_STREAM, 0);
	if (listen_sd < 0) {
		throw std::runtime_error("socket() failed");
	}

	int flags = fcntl(listen_sd, F_GETFL, 0);
	if (flags < 0) {
		close(listen_sd);
		throw std::runtime_error("fcntl() failed");
	}
	flags |= O_NONBLOCK;
	if (fcntl(listen_sd, F_SETFL, flags) < 0) {
		close(listen_sd);
		throw std::runtime_error("fcntl() failed to set non-blocking");
	}

	struct sockaddr_in addr = {};
	addr.sin_family = PF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);  // IPv4アドレスを指定
	addr.sin_port = htons(config_server.port);  // ポート番号を設定

	if (bind(listen_sd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
		close(listen_sd);
		throw std::runtime_error("bind() failed");
	}

	if (listen(listen_sd, 5) < 0) {
		close(listen_sd);
		throw std::runtime_error("listen() failed");
	}

	FD_ZERO(&master_set);
	// listen_sdをmaster_setに追加
	FD_SET(listen_sd, &master_set);
	max_sd = listen_sd;
}


void Server::AcceptNewConnection() {
	int new_sd = accept(listen_sd, NULL, NULL);
	if (new_sd < 0) {
		if (errno != EWOULDBLOCK) {
			// ノンブロッキング操作がブロックされた
			throw std::runtime_error("accept() failed");
		}
	}
	else {
		FD_SET(new_sd, &master_set);
		if (new_sd > max_sd) {
			max_sd = new_sd;
		}
	}
}

// 接続が確立されたソケットと通信する
void Server::ProcessConnection(int socket) {
	bool close_conn = false;
	char buffer[80];
	int rc;

	while (true) {
		rc = recv(socket, buffer, sizeof(buffer), 0);
		if (rc < 0) {
			if (errno != EWOULDBLOCK) {
				std::cerr << "recv() failed: " << strerror(errno) << std::endl;
				close_conn = true;
			}
			break;
		}

		if (rc == 0) {
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
		if (rc < 0) {
			std::cerr << "send() failed: " << strerror(errno) << std::endl;
			close_conn = true;
			break;
		}
	}

	if (close_conn) {
		CloseConnection(socket);
	}
}





int Server::Start(ConfigServer config_server) {
	InitializeSocket(config_server);

	fd_set working_set;
	struct timeval timeout;
	int end_server = FALSE;

	while (end_server == FALSE) {
		memcpy(&working_set, &master_set, sizeof(master_set));
		timeout.tv_sec = 3 * 60; // タイムアウト値を3分に設定
		timeout.tv_usec = 0;

		std::cout << "Waiting on select()..." << std::endl;
		int rc = select(max_sd + 1, &working_set, NULL, NULL, &timeout);
		if (rc < 0) {
			std::cerr << "select() failed: " << strerror(errno) << std::endl;
			break;
		}
		else if (rc == 0) {
			std::cout << "select() timed out. End program." << std::endl;
			break;
		}

		// std::cout << "max_sd" << max_sd << std::endl;
		for (int i = 0; i <= max_sd; ++i) {
			// working_setの集合に特定のFD(i)が存在するかどうかを判別する
			if (FD_ISSET(i, &working_set)) {
				if (i == listen_sd) {
					AcceptNewConnection();
				}
				else {
					ProcessConnection(i);
				}
			}
		}
	}

	for (int i = 0; i <= max_sd; ++i) {
		if (FD_ISSET(i, &master_set)) {
			CloseConnection(i);
		}
	}
	return 0;
}
