#include "shell.h"

char *error_environ(char **args);
char *error_code_1(char **args);
char *error_code_2_exit(char **args);
char *error_code_2_cd(char **args);
char *error_code_2_syntax(char **args);

/**
 * error_environ - Generates an error message for show_environ errors.
 *
 * @args: An array of arguments passed to the command.
 *
 * Return: The error string.
 */
char *error_environ(char **args)
{
	char *error, *hist_str;
	int len;

	hist_str = _itoa(historyCounter);
	if (!hist_str)
		return (NULL);

	args--;
	len = _strlen(name) + _strlen(hist_str) + _strlen(args[0]) + 45;
	error = malloc(sizeof(char) * (len + 1));
	if (!error)
	{
		free(hist_str);
		return (NULL);
	}

	_strcpy(error, name);
	_strcat(error, ": ");
	_strcat(error, hist_str);
	_strcat(error, ": ");
	_strcat(error, args[0]);
	_strcat(error, ": Unable to add/remove from environment\n");

	free(hist_str);
	return (error);
}

/**
 * error_code_1 - Generates an error message for define_alias errors.
 *
 * @args: An array of arguments passed to the command.
 *
 * Return: The error string.
 */
char *error_code_1(char **args)
{
	char *error;
	int len;

	len = _strlen(name) + _strlen(args[0]) + 13;
	error = malloc(sizeof(char) * (len + 1));
	if (!error)
		return (NULL);

	_strcpy(error, "alias: ");
	_strcat(error, args[0]);
	_strcat(error, " not found\n");

	return (error);
}

/**
 * error_code_2_exit - Generates an error message for exit_shell errors.
 *
 * @args: An array of arguments passed to the command.
 *
 * Return: The error string.
 */
char *error_code_2_exit(char **args)
{
	char *error, *hist_str;
	int len;

	hist_str = _itoa(historyCounter);
	if (!hist_str)
		return (NULL);

	len = _strlen(name) + _strlen(hist_str) + _strlen(args[0]) + 27;
	error = malloc(sizeof(char) * (len + 1));
	if (!error)
	{
		free(hist_str);
		return (NULL);
	}

	_strcpy(error, name);
	_strcat(error, ": ");
	_strcat(error, hist_str);
	_strcat(error, ": exit: Illegal number: ");
	_strcat(error, args[0]);
	_strcat(error, "\n");

	free(hist_str);
	return (error);
}

/**
 * error_code_2_cd - Generates an error message for change_dir errors.
 *
 * @args: An array of arguments passed to the command.
 *
 * Return: The error string.
 */
char *error_code_2_cd(char **args)
{
	char *error, *hist_str;
	int len;

	hist_str = _itoa(historyCounter);
	if (!hist_str)
		return (NULL);

	if (args[0][0] == '-')
		args[0][2] = '\0';
	len = _strlen(name) + _strlen(hist_str) + _strlen(args[0]) + 24;
	error = malloc(sizeof(char) * (len + 1));
	if (!error)
	{
		free(hist_str);
		return (NULL);
	}

	_strcpy(error, name);
	_strcat(error, ": ");
	_strcat(error, hist_str);
	if (args[0][0] == '-')
		_strcat(error, ": cd: Illegal option ");
	else
		_strcat(error, ": cd: can't cd to ");
	_strcat(error, args[0]);
	_strcat(error, "\n");

	free(hist_str);
	return (error);
}


/**
 * error_code_2_syntax - Generates an error message for syntax errors.
 *
 * @args: An array of arguments passed to the command.
 *
 * Return: The error string.
 */
char *error_code_2_syntax(char **args)
{
	char *error, *hist_str;
	int len;

	hist_str = _itoa(historyCounter);
	if (!hist_str)
		return (NULL);

	len = _strlen(name) + _strlen(hist_str) + _strlen(args[0]) + 33;
	error = malloc(sizeof(char) * (len + 1));
	if (!error)
	{
		free(hist_str);
		return (NULL);
	}

	_strcpy(error, name);
	_strcat(error, ": ");
	_strcat(error, hist_str);
	_strcat(error, ": Syntax error: \"");
	_strcat(error, args[0]);
	_strcat(error, "\" unexpected\n");

	free(hist_str);
	return (error);
}
