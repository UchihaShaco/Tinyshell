// pipe printf to the parent process
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdarg.h>
#include <limits.h>
#include <sys/stat.h>

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
// void	put_strs_fd(int num_str, ...)
// {
// 	int		i;
// 	int		fd;
// 	va_list	str;

// 	va_start(str, num_str);
// 	i = -1;
// 	fd = va_arg(str, int);
// 	while (++i < num_str)
// 	{
// 		if (putstr_fd(va_arg(str, char *), fd) == -1)
// 			return ;
// 	}
// 	va_end(str);
// }

long long check_exit_val(const char *str) {
    unsigned long long num = 0;
    int neg = 1;
    int i = 0;

    if (str[i] == '+' || str[i] == '-') {
        if (str[i] == '-')
            neg *= -1;
        i++;
    }
    while (str[i]) {
        if (num > ULLONG_MAX / 10) {
            printf("error\n");
            return (2);
        }
        num = (num * 10) + (str[i] - '0');
        i++;
    }

    if (neg == 1 && num > LLONG_MAX) {
		printf("pos num larger than LLONG_MAX\n");
        printf("error\n");
        return (2);
    }
    else if (neg == -1 && num > ((unsigned long long)LLONG_MAX + 1)) {
		printf("neg num larger than LLONG_MIN\n");
        printf("error\n");
        return (2);
    }

    printf("num is: %llu\n", num);

    return (long long)(num * neg);
}

int is_directory(const char *path) {
    struct stat path_stat;
    if (stat(path, &path_stat) != 0) {
        // error occurred, could not get file status
        return 0;
    }
    return S_ISDIR(path_stat.st_mode);
}

int main(int argc, char **argv, char **envp)
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

	// put_strs_fd(1, 2, "hello world on stderr\n");
	// put_strs_fd(1, 1, "hello world on stdout\n");
	// put_strs_fd()
	// write (2, "hello\n", 6);

	// char *str =  "9223372036854775809";
	// printf("%llu\n", check_exit_val(str));
	// int result = is_directory("/path/to/directory");
	// if (result == 1) 
	// 	printf("This is a directory\n");
	// else 
	// 	printf("This is not a directory\n");

	// struct stat file_stat;
	// int			stat_res;
	// int			permission; 

	// stat_res = stat("trash/file.txt", &file_stat);
	// if (stat_res == -1)
	// {
	// 	printf("no such file exists\n");
	// 	return (0);
	// }
	// permission = (file_stat.st_mode & S_IRUSR);
	// // else if (cmd->redir[i] == 3 || cmd->redir == 4)
	// // 	permission = (file_stat.st_mode && S_IWUSR);
	// printf("%i\n", permission);
	char *str = "./file/file";
	printf("%i\n", open(str, O_CREAT | O_RDWR | O_TRUNC, 0666));
}