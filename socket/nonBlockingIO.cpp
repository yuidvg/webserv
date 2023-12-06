#include "socket.hpp"

void nonBlockingIO(int pipefd[2])
{
	while (true)
	{
		// 親プロセス
		fd_set readfds;
		FD_ZERO(&readfds);
		FD_SET(pipefd[0], &readfds);

		int result = select(pipefd[0] + 1, &readfds, NULL, NULL, NULL);
		if (result > 0 && FD_ISSET(pipefd[0], &readfds))
		{
			char buffer[1024];
			ssize_t bytesRead = read(pipefd[0], buffer, sizeof(buffer));
			std::cout << "読み込んだバイト数: " << bytesRead << std::endl;
			if (bytesRead > 0)
			{
				buffer[bytesRead] = '\0';
				std::string str = buffer;

				// 改行文字でメッセージを分割
				size_t pos;
				while ((pos = str.find("\n")) != std::string::npos)
				{
					std::string msg = str.substr(0, pos);
					std::cout << "Received: " << msg << std::endl;
					str.erase(0, pos + 1);
				}
			}
			else
			{
				std::cerr << "Read error" << std::endl;
				break;
			}
		}
		else if (result == -1)
		{
			std::cout << "\x1b[31m読み込み可能ファイル無し\x1b[0m" << std::endl;
			break;
		}
		else if (result == 0)
		{
			std::cout << "\x1b[31mTime Out\x1b[0m" << std::endl;
			break;
		}
	}
}