#include "shell.h"

/**
 * handle_variables - handles variables passed to the shell
 * @hsh: shell data
 *
 * Return: the updated command with variables expanded if one was found, else
 * the @command is returned as received with no modifications
 */
char **handle_variables(shell_t *hsh)
{
	size_t i;
	ssize_t offset;
	char *value, *loc, result[32];

	for (i = 0; hsh->sub_command[i] != NULL; i++)
	{
		loc = _strchr(hsh->sub_command[i], '$');
		if (loc == NULL)
			/* no variables found yet? keep, searching till the end */
			continue;

		offset = (&loc[0]) - (&hsh->sub_command[i][0]);

		if (hsh->sub_command[i][offset + 1] == '\0')
			continue; /* this is just a literal '$' sign, ignore it */

		/* what's my PID? that's what this guy tells you */
		if (hsh->sub_command[i][offset + 1] == '$')
		{
			_itoa(getpid(), result);
			safe_free(hsh->sub_command[i]);
			hsh->sub_command[i] = _strdup(result);
		}
		/* this is for when the user wants the exit code of the last process */
		else if (hsh->sub_command[i][offset + 1] == '?')
		{
			_itoa(hsh->exit_code, result);
			safe_free(hsh->sub_command[i]);
			hsh->sub_command[i] = _strdup(result);
		}
		/* this is for when something like '$HOSTNAME' is received */
		else
		{
			value = _getenv(loc + 1);
			safe_free(hsh->sub_command[i]);
			hsh->sub_command[i] = _strdup(value);
		}
	}

	return (hsh->sub_command);
}
