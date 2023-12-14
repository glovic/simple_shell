#include "shell.h"

/**
 * main - the entry point for the shell
 * @argc: command line arguments counter
 * @argv: command line arguments
 *
 * Return: 0 on success, or the exit code of the just exited process
 */
int main(int argc, char *argv[])
{
	size_t len = 0;
	ssize_t n_read = 0;
	shell_t *hsh = NULL;

	/* scout for singal interrupts (Ctrl + C) */
	signal(SIGINT, sigint_handler);

	hsh = init_shell();
	hsh->prog_name = argv[0];
	build_path(&hsh->path_list);

	if (argc >= 2)
	{
		handle_file_as_input(argv[1], hsh);
	}

	while (RUNNING)
	{
		show_prompt();
		fflush(stdout);

		n_read = _getline(&hsh->line, &len, STDIN_FILENO);

		++hsh->cmd_count; /* keep track of the number of inputs to the shell */

		/* check for empty prompt or if Ctrl+D was received */
		if (n_read == 0)
		{
			if (isatty(STDIN_FILENO))
				printf("exit\n");
			handle_exit(hsh, multi_free); /* clean up and leave */
		}

		hsh->exit_code = parse_line(hsh);
		safe_free(hsh->line);
	}

	return (hsh->exit_code);
}
