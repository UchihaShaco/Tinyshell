# Tinyshell  <br>
Minishell is a small shell program that provides a command-line interface to interact with the underlying operating system. It is designed to meet the following requirements: <br>

# Requirements <br>
Prompt <br>
The shell should display a prompt when waiting for a new command. The prompt can be customized as desired. <br>

# Command History <br>
The shell should have a working history that allows users to recall previously executed commands. <br>

# Command Execution <br>
The shell should search for and launch the right executable based on the PATH variable or using a relative or absolute path. <br>

# Global Variables <br>
The shell should not use more than one global variable to avoid any unintended side-effects. The purpose of the global variable should be well-defined and documented. <br>

# Quoting <br>
The shell should not interpret unclosed quotes or special characters that are not required by the subject such as \ (backslash) or ; (semicolon). It should handle single and double quotes as well. <br>

# Redirection <br>
The shell should implement redirections such as < (input), > (output), << (here-doc), and >> (append). <br>

# Pipes <br>
The shell should implement pipes (| character) to connect the output of one command to the input of the next command in the pipeline. <br>

# Environment Variables <br>
The shell should handle environment variables (e.g., $HOME) which should expand to their values. <br>

# Exit Status <br>
The shell should handle $? which should expand to the exit status of the most recently executed foreground pipeline. <br>

# Signals <br>
The shell should handle signals such as ctrl-C, ctrl-D, and ctrl-\ which should behave like in bash. <br>

# Interactive Mode <br>
In interactive mode, the shell should display a new prompt on a new line when the user presses ctrl-C, exit when the user presses ctrl-D, and do nothing when the user presses ctrl-. <br>

# Functions <br>
The shell should use functions from the readline library such as readline, rl_clear_history, rl_on_new_line, rl_replace_line, rl_redisplay, and add_history to implement the command history and line editing features. <br>

# Memory Management <br>
The shell should use memory management functions such as malloc and free to allocate and deallocate memory as needed. <br>

# File I/O <br>
The shell should use file I/O functions such as access, open, read, and close to read and write files as needed. <br>

# Process Management <br>
The shell should use process management functions such as fork, wait, waitpid, wait3, wait4, signal, sigaction, sigemptyset, sigaddset, kill, and exit to create and manage child processes as needed. <br>

# Directory Management <br>
The shell should use directory management functions such as opendir, readdir, and closedir to list and manipulate directories as needed. <br>

# Error Handling <br>
The shell handles errors by using error handling functions such as strerror and perror to display error messages when necessary.<br> When an error occurs, the shell will display a descriptive error message indicating the source of the error and any relevant information.<br> This helps users quickly identify and resolve issues that may arise while using the shell. Additionally, Minishell uses a consistent approach to error handling throughout its implementation,  ensuring that errors are handled in a reliable and predictable manner. <br>

# Terminal Management <br>
The shell should use terminal management functions such as isatty, ttyname, ttyslot, ioctl, tcsetattr, tcgetattr, tgetent, tgetflag, tgetnum, tgetstr, tgoto, and tputs to manage the terminal as needed. <br>

# Built-in Commands <br>
The shell should implement the following built-in commands: <br>

* echo with option -n <br>
* cd with only a relative or absolute path <br>
* pwd with no options <br>
* export with no options <br>
* unset with no options <br>
* env with no options or arguments <br>
* exit with no options <br>
* By meeting these requirements, Minishell provides a robust and flexible shell program that can be used to interact with the operating system in a variety of ways. <br>

# Implementation <br>
Minishell is implemented in C programming language and uses various libraries and system calls to achieve its functionality. The program is structured using modular programming techniques, where different components of the shell are implemented in separate files to improve readability and maintainability.<br>

The readline library is used to implement the command history and line editing features of the shell. It provides functions such as readline, rl_clear_history, rl_on_new_line, rl_replace_line, rl_redisplay, and add_history. These functions are used to read user input from the terminal, store it in the history, and display it back to the user for editing.<br>

The shell uses the fork system call to create child processes to execute commands. The parent process waits for the child process to complete before proceeding. The wait system call is used to wait for the child process to exit and obtain its exit status. The kill system call is used to send signals to child processes as needed.<br>

The shell uses the execve system call to execute external commands. The PATH environment variable is used to search for the executable file in the directories specified in the variable. The shell also handles redirections such as < (input), > (output), << (here-doc), and >> (append) using the open, close, and dup2 system calls.<br>

The shell handles environment variables by expanding them to their values using the getenv system call. The shell also implements built-in commands such as echo with option -n, cd with only a relative or absolute path, pwd with no options, export with no options, unset with no options, env with no options or arguments, and exit with no options.<br>
# Conclusion

Minishell is a small but powerful shell program that provides a command-line interface to interact with the underlying operating system.<br> It is designed to meet the needs of developers, system administrators, and power users who require a flexible and customizable shell for their daily work. <br>

Minishell provides a rich set of features including command history, command execution, redirection, pipes, environment variables, exit status, signals, interactive mode, functions, memory management, file I/O, process management, directory management, error handling, terminal management, and built-in commands. <br>

The implementation of Minishell is modular and well-structured, making it easy to read, maintain, and extend. <br>The use of libraries and system calls allows the program to achieve its functionality efficiently and effectively. <br>

In conclusion, Minishell is a valuable tool for anyone who works with the command line on a regular basis. Its flexibility and power make it an excellent choice for developers, system administrators, and power users alike. <br>
