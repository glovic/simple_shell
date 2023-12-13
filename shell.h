#ifndef SHELL_H
#define SHELL_H

#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>

#define END_OF_FILE -2
#define EXIT -3

/* Global environment */
extern char **environ;
/* Global program name */
char *name;
/* Global history counter */
int historyCounter;

/**
 * struct list_s - A new struct type defining a linked list.
 * @dir: A directory path.
 * @next: A pointer to another struct list_s.
 */
typedef struct list_s
{
	char *dir;
	struct list_s *next;
} list_t;

/**
 * struct builtin_s - A new struct type defining builtin commands.
 * @name: The name of the builtin command.
 * @f: A function pointer to the builtin command's function.
 */
typedef struct builtin_s
{
	char *name;
	int (*f)(char **argv, char **front);
} builtin_t;

/**
 * struct alias_s - A new struct defining aliases.
 * @name: The name of the alias.
 * @value: The value of the alias.
 * @next: A pointer to another struct alias_s.
 */
typedef struct alias_s
{
	char *name;
	char *value;
	struct alias_s *next;
} alias_t;

/* Global aliases linked list */
alias_t *aliases;

/* Main Helpers */
ssize_t get_input_line(char **lineptr, size_t *n, FILE *stream);
void *_realloc(void *ptr, unsigned int old_size, unsigned int new_size);
char **tokenize_string(char *line, char *delim);
char *get_cmd_loc(char *cmd);
list_t *get_dir_path(char *path);
int exe_cmd(char **args, char **front);
void free_dir_list(list_t *head);
char *_itoa(int num);

/* Input Helpers */
void process_input_line(char **line, ssize_t read);
void replace_variables(char **args, int *exe_ret);
char *get_args(char *line, int *exe_ret);
int execute_args(char **args, char **front, int *exe_ret);
int run_args(char **args, char **front, int *exe_ret);
int handle_args(int *exe_ret);
int verify_args(char **args);
void free_exe_args(char **args, char **front);
char **substitute_aliases(char **args);

/* String functions */
int _strlen(const char *s);
char *_strcat(char *dest, const char *src);
char *_strncat(char *dest, const char *src, size_t n);
char *_strcpy(char *dest, const char *src);
char *_strchr(const char *s, char c);
int _strspn(char *s, char *accept);
int _strcmp(char *s1, char *s2);
int _strncmp(const char *s1, const char *s2, size_t n);

/* Builtins */
int (*get_builtin_cmd(char *cmd))(char **args, char **front);
int exit_shell(char **args, char **front);
int show_environ(char **args, char __attribute__((__unused__)) **front);
int set_environ_var(char **args, char __attribute__((__unused__)) **front);
int unset_environ_var(char **args, char __attribute__((__unused__)) **front);
int change_dir(char **args, char __attribute__((__unused__)) **front);
int define_alias(char **args, char __attribute__((__unused__)) **front);
int display_help(char **args, char __attribute__((__unused__)) **front);

/* Builtin Helpers */
char **copy_environ(void);
void free_environ(void);
char **get_environ_var(const char *var);

/* Error Handling */
int generate_error(char **args, int err);
char *error_environ(char **args);
char *error_code_1(char **args);
char *error_code_2_exit(char **args);
char *error_code_2_cd(char **args);
char *error_code_2_syntax(char **args);
char *error_code_126(char **args);
char *error_code_127(char **args);

/* Linkedlist Helpers */
alias_t *alias_to_end(alias_t **head, char *name, char *value);
void free_alias_list(alias_t *head);
list_t *add_dir_end(list_t **head, char *dir);
void free_dir_list(list_t *head);

void help_for_all(void);
void help_for_alias(void);
void help_for_cd(void);
void help_for_exit(void);
void help_for_help(void);
void help_for_environ(void);
void help_for_setenv(void);
void help_for_unsetenv(void);
void help_for_history(void);

int proc_cmd_file(char *file_path, int *exe_ret);

#endif /* SHELL_H */
