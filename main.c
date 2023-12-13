#include "shell.h"

void signalHandler(int sig);
int exe_cmd(char **args, char **front);

/**
 * signalHandler - Prints a new prompt upon a signal received.
 * @sig: The signal received.
 */
void signalHandler(int sig)
{
	char *new_prompt = "\n$ ";

	(void)sig;
	signal(SIGINT, signalHandler);
	write(STDIN_FILENO, new_prompt, 3);
}

/**
 * exe_cmd - Executes a command in a child process.
 * @args: An array of arguments.
 * @front: A double pointer to the start of args.
 *
 * Return: On error - Corresponding error code.
 *         Otherwise - Exit value of the last executed command.
 */
int exe_cmd(char **args, char **front)
{
	pid_t child_pid;
	int status, flag = 0, retu = 0;
	char *cmd = args[0];

	if (cmd[0] != '/' && cmd[0] != '.')
	{
		flag = 1;
		cmd = get_cmd_loc(cmd);
	}

	if (!cmd || (access(cmd, F_OK) == -1))
	{
		if (errno == EACCES)
			retu = (generate_error(args, 126));
		else
			retu = (generate_error(args, 127));
	}
	else
	{
		child_pid = fork();
		if (child_pid == -1)
		{
			if (flag)
				free(cmd);
			perror("Error child:");
			return (1);
		}
		if (child_pid == 0)
		{
			execve(cmd, args, environ);
			if (errno == EACCES)
				retu = (generate_error(args, 126));
			free_environ();
			free_exe_args(args, front);
			free_alias_list(aliases);
			_exit(retu);
		}
		else
		{
			wait(&status);
			retu = WEXITSTATUS(status);
		}
	}
	if (flag)
		free(cmd);
	return (retu);
}

/**
 * main - Executes a basic UNIX command interpreter.
 * @argc: Number of arguments passed to the program.
 * @argv: Array of pointers to the arguments.
 *
 * Return: Return value of the last executed command.
 */
int main(int argc, char *argv[])
{
	int retu = 0;
	int exe_ret = 0;
	char *prompt = "$ ", *new_line = "\n";

	name = argv[0];
	historyCounter = 1;
	aliases = NULL;
	signal(SIGINT, signalHandler);

	exe_ret = 0;
	environ = copy_environ();
	if (!environ)
		exit(-100);

	if (argc != 1)
	{
		retu = proc_cmd_file(argv[1], &exe_ret);
		free_environ();
		free_alias_list(aliases);
		return (exe_ret);
	}

	while (1)
	{
		write(STDOUT_FILENO, prompt, 2);
		retu = handle_args(&exe_ret);
		if (retu == END_OF_FILE || retu == EXIT)
		{
			if (retu == END_OF_FILE)
				write(STDOUT_FILENO, new_line, 1);
			free_environ();
			free_alias_list(aliases);
			exit(exe_ret);
		}
	}

	free_environ();
	free_alias_list(aliases);
	return (exe_ret);
}
