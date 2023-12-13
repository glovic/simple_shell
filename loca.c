#include "shell.h"

char *fill_path_dir(char *path);
list_t *get_path_dir(char *path);

/**
 * get_cmd_loc - Retrieves the full pathname of a command found in the PATH.
 *
 * @cmd: The command to locate.
 *
 * Return:
 *   - On success: A pointer to the full pathname of the command.
 *   - On failure: NULL if an error occurs or the command cannot be located.
 */
char *get_cmd_loc(char *cmd)
{
	char **path, *temp;
	list_t *dirs, *head;
	struct stat st;

	path = get_environ_var("PATH");
	if (!path || !(*path))
		return (NULL);

	dirs = get_path_dir(*path + 5);
	head = dirs;

	while (dirs)
	{
		temp = malloc(_strlen(dirs->dir) + _strlen(cmd) + 2);
		if (!temp)
			return (NULL);

		_strcpy(temp, dirs->dir);
		_strcat(temp, "/");
		_strcat(temp, cmd);

		if (stat(temp, &st) == 0)
		{
			free_dir_list(head);
			return (temp);
		}

		dirs = dirs->next;
		free(temp);
	}

	free_dir_list(head);

	return (NULL);
}

/**
 * fill_path_dir - Replaces leading, sandwiched, or trailing colons
 *                 in a path with the current working directory.
 *
 * @path: The colon-separated list of directories.
 *
 * Return: A copy of 'path' with any leading, sandwiched, or trailing
 *         colons replaced by the current working directory.
 */
char *fill_path_dir(char *path)
{
	int i, length = 0;
	char *path_copy, *pwd;

	pwd = *(get_environ_var("PWD")) + 4;
	for (i = 0; path[i]; i++)
	{
		if (path[i] == ':')
		{
			if (path[i + 1] == ':' || i == 0 || path[i + 1] == '\0')
				length += _strlen(pwd) + 1;
			else
				length++;
		}
		else
			length++;
	}
	path_copy = malloc(sizeof(char) * (length + 1));
	if (!path_copy)
		return (NULL);
	path_copy[0] = '\0';
	for (i = 0; path[i]; i++)
	{
		if (path[i] == ':')
		{
			if (i == 0)
			{
				_strcat(path_copy, pwd);
				_strcat(path_copy, ":");
			}
			else if (path[i + 1] == ':' || path[i + 1] == '\0')
			{
				_strcat(path_copy, ":");
				_strcat(path_copy, pwd);
			}
			else
				_strcat(path_copy, ":");
		}
		else
		{
			_strncat(path_copy, &path[i], 1);
		}
	}
	return (path_copy);
}

/**
 * get_path_dir - Converts a colon-separated list of directories
 *                into a linked list.
 *
 * @path: The colon-separated list of directories.
 *
 * Return: A pointer to the initialized linked list.
 */
list_t *get_path_dir(char *path)
{
	int index;
	char **dirs, *path_copy;
	list_t *head = NULL;

	path_copy = fill_path_dir(path);
	if (!path_copy)
		return (NULL);
	dirs = tokenize_string(path_copy, ":");
	free(path_copy);
	if (!dirs)
		return (NULL);

	for (index = 0; dirs[index]; index++)
	{
		if (add_dir_end(&head, dirs[index]) == NULL)
		{
			free_dir_list(head);
			free(dirs);
			return (NULL);
		}
	}

	free(dirs);

	return (head);
}
