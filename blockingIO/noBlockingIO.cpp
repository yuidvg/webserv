#include <cerrno>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <sys/select.h>
#include <unistd.h>

int main()
{
	int pipefd[2];
	pipe(pipefd);

	// ノンブロッキングの設定
	int flags = fcntl(pipefd[0], F_GETFL, 0);
	fcntl(pipefd[0], F_SETFL, flags | O_NONBLOCK);

	pid_t pid = fork();
	if (pid == 0)
	{
		sleep(3);
		const char *message = "Hello from child!";
		write(pipefd[1], message, strlen(message) + 1);
		close(pipefd[0]);
		close(pipefd[1]);
		exit(0);
	}
	else
	{
		fd_set readfds;
		FD_ZERO(&readfds);
		FD_SET(pipefd[0], &readfds);

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
				std::cerr << "Read error" << std::endl;
		}

		close(pipefd[0]);
		close(pipefd[1]);
	}
	return (0);
}
