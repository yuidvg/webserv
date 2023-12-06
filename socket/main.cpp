#include "socket.hpp"

int main()
{
	if (initializeSocket() == -1)
		return (-1);
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

	nonBlockingIO(pipefd);

	close(pipefd[0]);
	close(pipefd[1]);
	return (0);
}
