#include <cstring>
#include <iostream>
#include <unistd.h>

int main()
{
	int pipefd[2];
	pipe(pipefd);

	pid_t pid = fork();
	if (pid == 0)
	{
		// 子プロセス
		sleep(3);
		const char *message = "Hello from child!";
		write(pipefd[1], message, strlen(message) + 1);
		close(pipefd[0]);
		close(pipefd[1]);
		exit(0);
	}
	else
	{
		// 親プロセス
		char buffer[1024];
		ssize_t byteRead;

		std::cout << "Waiting for data..." << std::endl;

		// データが入力されるまで、待つ。
		byteRead = read(pipefd[0], buffer, sizeof(buffer));
		if (byteRead > 0)
		{
			std::cout << "Received: " << buffer << std::endl;
		}
		else
		{
			std::cerr << "Read error" << std::endl;
		}

		close(pipefd[0]);
		close(pipefd[1]);
	}
}