#include "shell.h"

void help_for_environ(void);
void help_for_setenv(void);
void help_for_unsetenv(void);
void help_for_history(void);



/**
 * help_for_environ - Display help information for the 'environ' option
 *                     of the 'show' builtin command.
 */
void help_for_environ(void)
{
	char *msg = "env: env\n\tPrints the current environment.\n";

	write(STDOUT_FILENO, msg, _strlen(msg));
}


/**
 * help_for_setenv - Provides help information for the 'setenv' option
 *                    of the set_environ builtin command.
 */
void help_for_setenv(void)
{
	char *msg = "setenv: setenv [VARIABLE] [VALUE]\n\tInitializes a new";

	write(STDOUT_FILENO, msg, _strlen(msg));
	msg = "environment variable, or modifies an existing one.\n\n";
	write(STDOUT_FILENO, msg, _strlen(msg));
	msg = "\tUpon failure, prints a message to stderr.\n";
	write(STDOUT_FILENO, msg, _strlen(msg));
}



/**
 * help_for_unsetenv - Displays infirmation on the builtin command
 * 'unsetenv'.
 */
void help_for_unsetenv(void)
{
	char *msg = "unsetenv: unsetenv [VARIABLE]\n\tRemoves an ";

	write(STDOUT_FILENO, msg, _strlen(msg));
	msg = "environmental variable.\n\n\tUpon failure, prints a ";
	write(STDOUT_FILENO, msg, _strlen(msg));
	msg = "message to stderr.\n";
	write(STDOUT_FILENO, msg, _strlen(msg));
}
