// pipe printf to the parent process
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdarg.h>

int	putstr_fd(char *s, int fd)
{
	if (!s)
		return (0);
	while (*s)
		if (write(fd, s++, 1) == -1)
			return (-1);
	return(0);
}
/* prints strings and accounts for printf errors */
void	put_strs_fd(int num_str, ...)
{
	int		i;
	int		fd;
	va_list	str;

	va_start(str, num_str);
	i = -1;
	fd = va_arg(str, int);
	while (++i < num_str)
	{
		if (putstr_fd(va_arg(str, char *), fd) == -1)
			return ;
	}
	va_end(str);
}

int main(void)
{
	// int	fd[2];
	// int	pid;

	// pipe(fd);
	// pid = fork();
	// if (pid == 0)
	// {
	// 	dup2(fd[1], STDOUT_FILENO);
	// 	close(fd[1]);
	// 	close (fd[0]);
	// 	printf("HELLO");
	// 	exit(0);
	// }
	// else
	// {
	// 	close(fd[1]);
	// 	wait(NULL);
	// 	char buffer[256];
	// 	read(fd[0], buffer, 3);
	// 	close(fd[0]);
	// 	printf("%s\n", buffer);
	// }
	// int fd;

	// fd = open("file3", O_WRONLY | O_CREAT, 0666);
	// dup2(fd, STDOUT_FILENO);
	// close(fd);
	// printf("hello\n");

	put_strs_fd(1, 2, "hello world on stderr\n");
	// put_strs_fd(1, 1, "hello world on stdout\n");
	// put_strs_fd()
	// write (2, "hello\n", 6);
}