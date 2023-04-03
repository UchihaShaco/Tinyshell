valgrind --suppressions=valgrind_readline_leaks_ignore.txt --leak-check=full --trace-children=yes --show-leak-kinds=all --track-origins=yes --track-fds=yes ./minishell
