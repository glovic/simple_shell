#include "shell.h"

char *get_args(char *line, int *exe_ret);
int execute_args(char **args, char **front, int *exe_ret);
int run_args(char **args, char **front, int *exe_ret);
int handle_args(int *exe_ret);
int verify_args(char **args);

/**
 * get_args - Reads a command from standard input and stores it in a buffer.
 *
 * @line: A buffer to store the command.
 * @exe_ret: The return value of the last executed command.
 *
 * Return:
 *   - On error: NULL.
 *   - Otherwise: A pointer to the stored command.
 */
char *get_args(char *line, int *exe_ret)
{
	size_t n = 0;
	ssize_t read;
	char *prompt = "$ ";

	if (line)
		free(line);

	read = get_input_line(&line, &n, STDIN_FILENO);
	if (read == -1)
		return (NULL);
	if (read == 1)
	{
		historyCounter++;
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, prompt, 2);
		return (get_args(line, exe_ret));
	}

	line[read - 1] = '\0';
	replace_variables(&line, exe_ret);
	process_input_line(&line, read);

	return (line);
}

/**
 * execute_args - Partitions operators from commands and calls them.
 * @args: An array of arguments.
 * @front: A double pointer to the beginning of args.
 * @exe_ret: The return value of the parent process' last executed command.
 *
 * Return: The return value of the last executed command.
 */
int execute_args(char **args, char **front, int *exe_ret)
{
	int retu, index;

	if (!args[0])
		return (*exe_ret);
	for (index = 0; args[index]; index++)
	{
		if (_strncmp(args[index], "||", 2) == 0)
		{
			free(args[index]);
			args[index] = NULL;
			args = substitute_aliases(args);
			retu = run_args(args, front, exe_ret);
			if (*exe_ret != 0)
			{
				args = &args[++index];
				index = 0;
			}
			else
			{
				for (index++; args[index]; index++)
					free(args[index]);
				return (retu);
			}
		}
		else if (_strncmp(args[index], "&&", 2) == 0)
		{
			free(args[index]);
			args[index] = NULL;
			args = substitute_aliases(args);
			retu = run_args(args, front, exe_ret);
			if (*exe_ret == 0)
			{
				args = &args[++index];
				index = 0;
			}
			else
			{
				for (index++; args[index]; index++)
					free(args[index]);
				return (retu);
			}
		}
	}
	args = substitute_aliases(args);
	retu = run_args(args, front, exe_ret);
	return (retu);
}

/**
 * run_args - Calls the execution of a command.
 * @args: An array of arguments.
 * @front: A double pointer to the beginning of args.
 * @exe_ret: The return value of the parent process' last executed command.
 *
 * Return: The return value of the last executed command.
 */
int run_args(char **args, char **front, int *exe_ret)
{
	int retu, i;
	int (*builtin)(char **args, char **front);

	builtin = get_builtin_cmd(args[0]);

	if (builtin)
	{
		retu = builtin(args + 1, front);
		if (retu != EXIT)
			*exe_ret = retu;
	}
	else
	{
		*exe_ret = exe_cmd(args, front);
		retu = *exe_ret;
	}

	historyCounter++;

	for (i = 0; args[i]; i++)
		free(args[i]);

	return (retu);
}

/**
 * handle_args - Gets, calls, and runs the execution of a command.
 * @exe_ret: The return value of the parent process' last executed command.
 *
 * Return: If an end-of-file is read - END_OF_FILE (-2).
 *         If the input cannot be tokenized - -1.
 *         O/w - The exit value of the last executed command.
 */
int handle_args(int *exe_ret)
{
	int retu = 0, index;
	char **args, *line = NULL, **front;

	line = get_args(line, exe_ret);
	if (!line)
		return (END_OF_FILE);

	args = tokenize_string(line, " ");
	free(line);
	if (!args)
		return (retu);
	if (verify_args(args) != 0)
	{
		*exe_ret = 2;
		free_exe_args(args, args);
		return (*exe_ret);
	}
	front = args;

	for (index = 0; args[index]; index++)
	{
		if (_strncmp(args[index], ";", 1) == 0)
		{
			free(args[index]);
			args[index] = NULL;
			retu = execute_args(args, front, exe_ret);
			args = &args[++index];
			index = 0;
		}
	}
	if (args)
		retu = execute_args(args, front, exe_ret);

	free(front);
	return (retu);
}

/**
 * verify_args - It verifies if there are any leading ';', ';;', '&&', or '||'.
 * @args: 2D pointer to tokenized commands and arguments.
 *
 * Return: If a ';', '&&', or '||' is placed at an invalid position - 2.
 *	   Otherwise - 0.
 */
int verify_args(char **args)
{
	size_t i;
	char *cur, *nex;

	for (i = 0; args[i]; i++)
	{
		cur = args[i];
		if (cur[0] == ';' || cur[0] == '&' || cur[0] == '|')
		{
			if (i == 0 || cur[1] == ';')
				return (generate_error(&args[i], 2));
			nex = args[i + 1];
			if (nex && (nex[0] == ';' || nex[0] == '&' || nex[0] == '|'))
				return (generate_error(&args[i + 1], 2));
		}
	}
	return (0);
}
