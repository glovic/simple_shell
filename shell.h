/* 
 * Authors: <Victor Nwaogu>, <David Onumonu>
 * Description: Header file for a shell program.
 * Date: <14/12/2023>
 */

#ifndef SHELL_H
#define SHELL_H

#include <errno.h>
#include <fcntl.h>
#include <regex.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

/* Macros - Preprocessor macros defining constants and utilities. */

#define BUFF_SIZE 1024
#define SPACE ' '
#define CMD_NOT_FOUND 127
#define PROMPT_SIZE 4096
#define PATH_SIZE 2048
#define NOT_BUILTIN 18
#define RUNNING 1
#define CMD_ERR 2
#define MAX_ALIAS_LENGTH 50
#define MAX_VALUE_LENGTH 2048

/* Function Macros - Macros related to function operations. */

#define isspace(c) ((c) == SPACE)
#define isdigit(c) (((c) >= '0' && (c) <= '9')) /* checks for digits */
#define isalpha(c) (((c) >= 'a' && (c) <= 'z') || ((c) >= 'A' && (c) <= 'Z'))
#define isnegative(c) (((c) == '-') ? -1 : 1)
#define issign(c) ((c) == '-' || (c) == '+')
#define isquote(c) ((c) == '"' || (c) == '\'')

/* String Handlers - Functions for handling string operations. */

size_t _strlen(const char *s);
char *_strdup(const char *str);
char *_strchr(const char *s, int c);
char *_strrchr(const char *s, int c);
char *_strcpy(char *dest, const char *src);
char *_strcat(char *dest, const char *src);
int _strcmp(const char *s1, const char *s2);
char **duplicate_str_array(char **original);
void concatenate_arrays(char ***dest, char **src);
char *_strpbrk(const char *s, const char *accept);
size_t _strspn(const char *s, const char *accept);
char **_strtok(const char *str, const char *delim);
char *_strncpy(char *dest, const char *src, size_t n);
int _strncmp(const char *s1, const char *s2, size_t n);
int get_word_count(const char *str, const char *delim);
char *_strstr(const char *haystack, const char *needle);

/* numbers */

int _atoi(const char *s);
void _itoa(size_t n, char *s);
void _reverse(char *buffer, size_t len);

/* memory handlers */

void free_str(char ***str_array);
char *new_word(const char *str, int start, int end);
void *_memcpy(void *dest, const void *src, size_t n);
void *_realloc(void *old_mem_blk, size_t old_size, size_t new_size);

/* a safer way to deallocate dynamic memory */
void _free(void **ptr);
#define safe_free(ptr) _free((void **)&(ptr)) /* _free's frontend */

/* frees memory for a variable number of objects */
void multi_free(const char *format, ...);

/* a custom implementation of the getline function */
ssize_t _getline(char **lineptr, size_t *n, int fd);

/* shows the prompt in interactive mode */
void show_prompt(void);

/* retrieves the hostname from the '/etc/hostname' file */
char *get_hostname(char *buffer);

/* environment and PATH handlers */

extern char **environ;

/**
 * struct _path - Structure to build a linked list of directories
 *                in the PATH variable.
 *
 * @pathname: Each path name in the PATH variable.
 * @next: Pointer to the next node in the linked list.
 */

typedef struct _path
{
	char *pathname;
	struct _path *next;
} path_t;

void _printenv(void);
void print_path(path_t *list);
void free_list(path_t **head);
char *_getenv(const char *name);
path_t *build_path(path_t **head);

/**
 * struct alias - Represents the structure for the built-in alias command.
 * @name: Name of the alias.
 * @value: Value assigned to the alias name.
 * @next: Pointer to the next alias_t node.
 */

typedef struct alias
{
	char *name;
	char *value;
	struct alias *next;
} alias_t;

void free_aliases(alias_t **aliases);
void print_aliases(const alias_t *aliases);
int unalias(alias_t **aliases, char *command);
char *get_alias(alias_t *aliases, const char *name);
int handle_alias(alias_t **aliases, char *command_line);
int print_alias(const alias_t *aliases, const char *name);
void parse_aliases(const char *input, alias_t **aliases);
void build_alias_cmd(char ***sub_command, char *alias_value);
alias_t *add_alias(alias_t **aliases, const char *name, const char *value);
void process_non_matching(alias_t *aliases, const char *non_matching, int end);


/**
 * struct shell - Structure defining the shell properties.
 * @aliases: A list of aliases.
 * @path_list: A list of directories in the PATH.
 * @line: The command string provided by the user.
 * @commands: Initial tokenized commands (split by semi-colons & newlines).
 * @sub_command: Tokenized version of each command in the commands array.
 * @prog_name: The name of the shell program.
 * @cmd_count: Number of times a command has been executed since shell start.
 * @tokens: Stores multiple tokens before further processing.
 * @token: A single token.
 * @exit_code: Exit code of the last executed program.
 */

typedef struct shell
{
	alias_t *aliases;
	path_t *path_list;
	char *line;
	char **commands;
	char **sub_command;
	char **tokens;
	char *token;
	const char *prog_name;
	size_t cmd_count;
	int exit_code;
} shell_t;

shell_t *init_shell(void);
void sigint_handler(int signum);

/**
 * struct builtin_handlers - Collection of functions for handling built-in commands.
 * @unsetenv: Unset an environment variable.
 * @handle_cd: Change directory.
 * @_setenv: Set an environment variable.
 * @handle_builtin: Execute built-in commands.
 * @handle_exit: Handle exit command.
 */

int _unsetenv(const char *name);
int handle_cd(shell_t *msh);
int _setenv(const char *name, const char *value, int overwrite);
int handle_builtin(shell_t *msh);
int handle_exit(shell_t *msh, void (*cleanup)(const char *format, ...));

/* parsers and executors */
/**
 * struct parsers_executors - Functions for parsing and executing commands.
 * @get_operator: Retrieve the operator in a command line.
 * @handle_comments: Handle comments in a command line.
 * @parse_line: Parse a command line into separate commands.
 * @execute_command: Execute a command with provided pathname.
 * @parse_and_execute: Parse and execute commands at a specified index.
 * @handle_with_path: Handle command execution with PATH directories.
 * @print_cmd_not_found: Print command not found error message.
 * @handle_file_as_input: Handle file input for command execution.
 * @handle_variables: Handle environment variable substitutions.
 * @parse: Main parsing function for commands.
 * @parse_helper: Helper function for parsing commands at an index.
 */

char *get_operator(char *str);
char *handle_comments(char *command);
int parse_line(shell_t *msh);
int execute_command(const char *pathname, shell_t *msh);
int parse_and_execute(shell_t *msh, size_t index);
int handle_with_path(shell_t *msh);
int print_cmd_not_found(shell_t *msh);
void handle_file_as_input(const char *filename, shell_t *msh);
char **handle_variables(shell_t *msh);
int parse(shell_t *msh);
void parse_helper(shell_t *msh, size_t index);
char *handle_comments(char *line);


#endif /* SHELL_H */
