#include <cerrno>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <sys/select.h>
#include <unistd.h>
#include <vector>

int main()
{
	int pipefd[2];
	pipe(pipefd);

	// ノンブロッキングの設定
	int flags = fcntl(pipefd[0], F_GETFL, 0);
	fcntl(pipefd[0], F_SETFL, flags | O_NONBLOCK);

	std::vector<const char *> message;
	message.push_back("\x1b[32m1\x1b[0m");
	message.push_back("\x1b[32m2\x1b[0m");
	message.push_back("\x1b[32m3\x1b[0m");

	for (int i = 0; i < message.size(); ++i)
	{
		pid_t pid = fork();
		if (pid == 0)
		{
			sleep(3);
			std::string msg_with_newline = std::string(message[i]) + "\n";
			write(pipefd[1], msg_with_newline.c_str(), msg_with_newline.size());
			close(pipefd[0]);
			close(pipefd[1]);
			exit(0);
		}
	}

	std::string buf;
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
				buf += buffer;

				// 改行文字でメッセージを分割
				size_t pos;
				while ((pos = buf.find("\n")) != std::string::npos)
				{
					std::string msg = buf.substr(0, pos);
					std::cout << "Received: " << msg << std::endl;
					buf.erase(0, pos + 1);
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
	close(pipefd[0]);
	close(pipefd[1]);
	return (0);
}
