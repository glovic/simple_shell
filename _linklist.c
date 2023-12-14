#include "shell.h"

alias_t *alias_to_end(alias_t **head, char *name, char *value);
void free_alias_list(alias_t *head);
list_t *add_dir_end(list_t **head, char *dir);
void free_dir_list(list_t *head);

/**
 * alias_to_end - Adds a node containing a new alias
 *                to the end of an alias_t linked list.
 *
 * @head: Pointer to the head of the alias_t list.
 * @name: The name of the new alias.
 * @value: The value of the new alias.
 *
 * Return: NULL on error.
 *         Otherwise, a pointer to the new node.
 */
alias_t *alias_to_end(alias_t **head, char *name, char *value)
{
	alias_t *new_node = malloc(sizeof(alias_t));
	alias_t *last;

	if (!new_node)
		return (NULL);

	new_node->next = NULL;
	new_node->name = malloc(sizeof(char) * (_strlen(name) + 1));
	if (!new_node->name)
	{
		free(new_node);
		return (NULL);
	}
	new_node->value = value;
	_strcpy(new_node->name, name);

	if (*head)
	{
		last = *head;
		while (last->next != NULL)
			last = last->next;
		last->next = new_node;
	}
	else
		*head = new_node;

	return (new_node);
}

/**
 * add_dir_end - Adds a node containing a directory path
 *                to the end of a list_t linked list.
 *
 * @head: Pointer to the head of the list_t list.
 * @dir: The directory path for the new node to contain.
 *
 * Return: NULL on error.
 *         Otherwise, a pointer to the new node.
 */
list_t *add_dir_end(list_t **head, char *dir)
{
	list_t *new_node = malloc(sizeof(list_t));
	list_t *last;

	if (!new_node)
		return (NULL);

	new_node->dir = dir;
	new_node->next = NULL;

	if (*head)
	{
		last = *head;
		while (last->next != NULL)
			last = last->next;
		last->next = new_node;
	}
	else
		*head = new_node;

	return (new_node);
}

/**
 * free_alias_list - Frees an alias_t linked list.
 *
 * @head: The head of the alias_t list.
 */
void free_alias_list(alias_t *head)
{
	alias_t *next;

	while (head)
	{
		next = head->next;
		free(head->name);
		free(head->value);
		free(head);
		head = next;
	}
}

/**
 * free_dir_list - Frees a list_t linked list.
 * @head: The head of the list_t list.
 */
void free_dir_list(list_t *head)
{
	list_t *next;

	while (head)
	{
		next = head->next;
		free(head->dir);
		free(head);
		head = next;
	}
}
