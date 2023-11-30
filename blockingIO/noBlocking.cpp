#include <cerrno>
#include <fcntl.h>
#include <iostream>
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
		char buffer[1024];
		ssize_t bytesRead;

		while (true)
		{
			bytesRead = read(pipefd[0], buffer, sizeof(buffer));
			if (bytesRead > 0)
			{
				std::cout << "Received: " << buffer << std::endl;
				break;
			}
			else if (bytesRead == -1 && (errno == EAGAIN || errno == EWOULDBLOCK))
			{
				std::cout << "No data yet. Trying again in 1 second..." << std::endl;
				sleep(1);
			}
			else
				break;
		}
		close(pipefd[0]);
		close(pipefd[1]);
	}

	return (0);
}
