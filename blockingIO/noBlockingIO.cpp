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
			write(pipefd[1], message[i], strlen(message[i]) + 1);
			close(pipefd[0]);
			close(pipefd[1]);
			exit(0);
		}
	}

	while (true)
	{
		// 親プロセス
		fd_set readfds;
		FD_ZERO(&readfds);
		FD_SET(pipefd[0], &readfds);
		// struct timeval tv;
		// tv.tv_sec = 3;
		// tv.tv_usec = 0;

		int result = select(pipefd[0] + 1, &readfds, NULL, NULL, NULL);
		if (result > 0 && FD_ISSET(pipefd[0], &readfds))
		{
			char buffer[1024];
			ssize_t bytesRead = read(pipefd[0], buffer, sizeof(buffer));
			if (bytesRead > 0)
			{
				buffer[bytesRead] = '\0';
				std::cout << "Received: " << buffer << std::endl;
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
