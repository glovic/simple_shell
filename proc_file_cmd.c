#include "shell.h"

/**
 * handle_with_path - handles commands when the PATH is set
 * @hsh: contains all the data relevant to the shell's operation
 *
 * Return: the exit code of the child process, else -1 if the command is not in
 * the PATH provided
 */
int handle_with_path(shell_t *hsh)
{
	char path[BUFF_SIZE];
	path_t *path_list = hsh->path_list;

	while (path_list != NULL)
	{
		sprintf(path, "%s%s%s", path_list->pathname, "/", hsh->sub_command[0]);
		if (access(path, X_OK) == 0)
		{
			return (execute_command(path, hsh));
		}
		else if (access(hsh->sub_command[0], X_OK) == 0)
		{
			return (execute_command(path, hsh));
		}
		path_list = path_list->next;
	}

	return (-1);
}

/**
 * handle_file_as_input - handles execution when a file is given as input on
 * the command line (non-interactive mode)
 * @filename: the name of the file to read from
 * @hsh: contains all the data relevant to the shell's operation
 *
 * Return: 0, or the exit status of the just exited process
 */
void handle_file_as_input(const char *filename, shell_t *hsh)
{
	size_t n = 0;
	int n_read, fd;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		/* we couldn't open the file, let's clean and leave */
		free_list(&hsh->path_list);
		fprintf(stderr, "%s: 0: Can't open %s\n", hsh->prog_name, filename);
		exit(CMD_NOT_FOUND);
	}

	n_read = _getline(&hsh->line, &n, fd);

	/*
	 * let us know if there was an error while closing file descriptor but
	 * continue any way
	 */
	if (close(fd) == -1)
		fprintf(stderr, "An error occurred while closing file descriptor #%d\n", fd);

	if (n_read == -1)
	{
		hsh->exit_code = -1;
		handle_exit(hsh, multi_free);
	}

	if (n_read)
	{
		hsh->prog_name = filename;
		parse_line(hsh);
	}

	handle_exit(hsh, multi_free);
}
