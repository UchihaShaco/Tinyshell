// pipe printf to the parent process
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(void)
{
	int	fd[2];
	int	pid;

	pipe(fd);
	pid = fork();
	if (pid == 0)
	{
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
		close (fd[0]);
		printf("HELLO");
		exit(0);
	}
	else
	{
		close(fd[1]);
		wait(NULL);
		char buffer[256];
		read(fd[0], buffer, 3);
		close(fd[0]);
		printf("%s\n", buffer);
	}
}