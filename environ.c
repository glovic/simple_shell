#include "shell.h"

char **copy_environ(void);
void free_environ(void);
char **get_environ_var(const char *var);

/**
 * copy_environ - Creates a copy of the environment.
 *
 * Return:
 *   - If an error occurs: NULL.
 *   - Otherwise: a double pointer to the new copy.
 */
char **copy_environ(void)
{
	char **new_environ;
	size_t size;
	int index;

	for (size = 0; environ[size]; size++)
		;

	new_environ = malloc(sizeof(char *) * (size + 1));
	if (!new_environ)
		return (NULL);

	for (index = 0; environ[index]; index++)
	{
		new_environ[index] = malloc(_strlen(environ[index]) + 1);

		if (!new_environ[index])
		{
			for (index--; index >= 0; index--)
				free(new_environ[index]);
			free(new_environ);
			return (NULL);
		}
		_strcpy(new_environ[index], environ[index]);
	}
	new_environ[index] = NULL;

	return (new_environ);
}

/**
 * free_environ - Frees the environment copy.
 */
void free_environ(void)
{
	int index;

	for (index = 0; environ[index]; index++)
		free(environ[index]);
	free(environ);
}

/**
 * get_environ_var - Gets an environmental variable from the PATH.
 *
 * @var: The name of the environmental variable to get.
 *
 * Return:
 *   - If the environmental variable does not exist: NULL.
 *   - Otherwise: a pointer to the environmental variable.
 */
char **get_environ_var(const char *var)
{
	int index, len;

	len = _strlen(var);
	for (index = 0; environ[index]; index++)
	{
		if (_strncmp(var, environ[index], len) == 0)
			return (&environ[index]);
	}

	return (NULL);
}
