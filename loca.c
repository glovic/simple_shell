#include "shell.h"

/**
 * parse_line - parses the receive command line, processes it before handing it
 * over to the executor only after it has confirmed the command is valid
 * @hsh: contains all the data relevant to the shell's operation
 *
 * Return: the exit code of the executed program, else -1 if something goes
 * wrong
 */
int parse_line(shell_t *hsh)
{
	size_t i;

	/* skip normal ENTER keys and leading comments */
	if (*hsh->line == '\n' || *hsh->line == '#')
		return (0);

	/* first of all, let's get rid of all comments */
	hsh->line = handle_comments(hsh->line);

	hsh->tokens = _strtok(hsh->line, "\n");
	if (hsh->tokens == NULL)
	{
		fprintf(stderr, "Not enough system memory to continue\n");
		return (-1);
	}

	for (i = 0; hsh->tokens[i] != NULL; i++)
	{
		hsh->token = hsh->tokens[i];

		if (!_strcmp(hsh->tokens[i], "exit") && hsh->tokens[i + 1] == NULL)
		{
			handle_exit(hsh, multi_free);
		}

		/* now let's tokenize all the commands provided by the user */
		hsh->commands = _strtok(hsh->token, ";\n");
		if (hsh->commands == NULL)
		{
			fprintf(stderr, "Memory allocation failed...\n");
			return (-1);
		}

		hsh->exit_code = parse(hsh);
		free_str(&hsh->commands);
	}

	free_str(&hsh->tokens);
	return (hsh->exit_code);
}

/**
 * parse - parses an array of commands received from the prompt
 * @hsh: contains all the data relevant to the shell's operation
 *
 * Return: the exit code of the executed program
 */
int parse(shell_t *hsh)
{
	ssize_t i, offset;
	char *cur_cmd = NULL, *operator = NULL;
	char *next_cmd = NULL, *temp_next_cmd = NULL;

	for (i = 0; hsh->commands[i] != NULL; i++)
	{
		operator = get_operator(hsh->commands[i]);
		if (operator != NULL)
		{
			offset = strcspn(hsh->commands[i], operator);
			/* extract the command before the operator */
			cur_cmd = strndup(hsh->commands[i], offset);
			if (cur_cmd == NULL)
				return (0);
			hsh->sub_command = _strtok(cur_cmd, NULL);
			safe_free(cur_cmd);
			if (hsh->sub_command == NULL)
				return (0);
			hsh->sub_command = handle_variables(hsh);
			parse_helper(hsh, i);

			temp_next_cmd = _strdup(&hsh->commands[i][offset + 2]);
			safe_free(next_cmd);
			safe_free(hsh->commands[i]);

			/* check the exit code and react accordingly */
			if ((!_strcmp(operator, "&&") && hsh->exit_code == 0) ||
				(!_strcmp(operator, "||") && hsh->exit_code != 0))
			{
				hsh->commands[i] = temp_next_cmd;
				parse(hsh);
				next_cmd = temp_next_cmd;
			}
			else
				safe_free(temp_next_cmd);
		}
		else
			parse_and_execute(hsh, i);
	}
	return (hsh->exit_code);
}

/**
 * parse_and_execute - parses each sub command line and executes it
 * @hsh: contains all the data relevant to the shell's operation
 * @index: the current index in commands array
 *
 * Return: the exit code of the executed program
 */
int parse_and_execute(shell_t *hsh, size_t index)
{
	/* get the sub commands and work on them */
	hsh->sub_command = _strtok(hsh->commands[index], NULL);
	if (hsh->sub_command == NULL)
	{
		return (0); /* probably just lots of tabs or spaces, maybe both */
	}

	/* check for variables */
	hsh->sub_command = handle_variables(hsh);
	if (hsh->sub_command[0] != NULL && hsh->sub_command != NULL)
		parse_helper(hsh, index);
	else
		free_str(&hsh->sub_command);

	/* cleanup and leave */
	safe_free(hsh->commands[index]);
	return (hsh->exit_code);
}

/**
 * parse_helper - performs extra parsing on behalf of the parse and execute
 * function
 * @hsh: contains all the data relevant to the shell's operation
 * @index: the current index in commands array
 */
void parse_helper(shell_t *hsh, size_t index)
{
	char *alias_value;

	if (!_strcmp(hsh->sub_command[0], "alias") ||
			!_strcmp(hsh->sub_command[0], "unalias"))
	{
		hsh->exit_code = handle_alias(&hsh->aliases, hsh->commands[index]);
		free_str(&hsh->sub_command);
		return;
	}

	alias_value = get_alias(hsh->aliases, hsh->sub_command[0]);
	if (alias_value != NULL)
	{
		build_alias_cmd(&hsh->sub_command, alias_value);
		safe_free(alias_value);
	}

	hsh->exit_code = handle_builtin(hsh);
	if (hsh->exit_code != NOT_BUILTIN)
	{
		free_str(&hsh->sub_command);
		return; /* shell builtin executed well */
	}
	/* handle the command with the PATH variable */
	if (hsh->path_list != NULL && !_strchr(hsh->sub_command[0], '/'))
	{
		hsh->exit_code = handle_with_path(hsh);
		if (hsh->exit_code == -1)
			hsh->exit_code = print_cmd_not_found(hsh);
	}
	else
	{
		if (access(hsh->sub_command[0], X_OK) == 0 &&
				_strchr(hsh->sub_command[0], '/'))
			hsh->exit_code = execute_command(hsh->sub_command[0], hsh);
		else
			hsh->exit_code = print_cmd_not_found(hsh);
	}
	free_str(&hsh->sub_command);
}

/**
 * print_cmd_not_found - prints the command not found error
 * @hsh: contains all the data relevant to the shell's operation
 *
 * Return: 127 command not found code, else 0
 */
int print_cmd_not_found(shell_t *hsh)
{
	dprintf(STDERR_FILENO, "%s: %lu: %s: not found\n", hsh->prog_name,
			hsh->cmd_count, hsh->sub_command[0]);

	return (CMD_NOT_FOUND); /* command not found */
}
