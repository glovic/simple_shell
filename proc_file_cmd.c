#include "shell.h"

int cant_open(char *file_path);
int proc_cmd_file(char *file_path, int *exe_ret);

/**
 * cant_open - Handles file opening failures.
 *
 * @file_path: Path to the intended file.
 *
 * Return: 127 indicating a failed file opening.
 */
int cant_open(char *file_path)
{
	char *error, *hist_str;
	int len;

	hist_str = _itoa(historyCounter);
	if (!hist_str)
		return (127);

	len = _strlen(name) + _strlen(hist_str) + _strlen(file_path) + 16;
	error = malloc(sizeof(char) * (len + 1));
	if (!error)
	{
		free(hist_str);
		return (127);
	}

	_strcpy(error, name);
	_strcat(error, ": ");
	_strcat(error, hist_str);
	_strcat(error, ": Can't open ");
	_strcat(error, file_path);
	_strcat(error, "\n");

	free(hist_str);
	write(STDERR_FILENO, error, len);
	free(error);
	return (127);
}

/**
 * proc_cmd_file - Executes commands stored in a file.
 *
 * @file_path: Path to the file.
 * @exe_ret: Return value of the last executed command.
 *
 * Return: 127 if the file couldn't be opened.
 *         -1 if malloc fails.
 *         Otherwise, the return value of the last command executed.
 */
int proc_cmd_file(char *file_path, int *exe_ret)
{
	ssize_t file, b_read, i;
	unsigned int line_size = 0;
	unsigned int old_size = 120;
	char *line, **args, **front;
	char buffer[120];
	int retu;

	historyCounter = 0;
	file = open(file_path, O_RDONLY);
	if (file == -1)
	{
		*exe_ret = cant_open(file_path);
		return (*exe_ret);
	}
	line = malloc(sizeof(char) * old_size);
	if (!line)
		return (-1);
	do {
		b_read = read(file, buffer, 119);
		if (b_read == 0 && line_size == 0)
			return (*exe_ret);
		buffer[b_read] = '\0';
		line_size += b_read;
		line = _realloc(line, old_size, line_size);
		_strcat(line, buffer);
		old_size = line_size;
	} while (b_read);
	for (i = 0; line[i] == '\n'; i++)
		line[i] = ' ';
	for (; i < line_size; i++)
	{
		if (line[i] == '\n')
		{
			line[i] = ';';
			for (i += 1; i < line_size && line[i] == '\n'; i++)
				line[i] = ' ';
		}
	}
	replace_variables(&line, exe_ret);
	process_input_line(&line, line_size);
	args = tokenize_string(line, " ");
	free(line);
	if (!args)
		return (0);
	if (verify_args(args) != 0)
	{
		*exe_ret = 2;
		free_exe_args(args, args);
		return (*exe_ret);
	}
	front = args;

	for (i = 0; args[i]; i++)
	{
		if (_strncmp(args[i], ";", 1) == 0)
		{
			free(args[i]);
			args[i] = NULL;
			retu = execute_args(args, front, exe_ret);
			args = &args[++i];
			i = 0;
		}
	}

	retu = execute_args(args, front, exe_ret);

	free(front);
	return (retu);
}
