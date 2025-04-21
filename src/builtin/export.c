/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnemmass <hnemmass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 14:13:32 by hnemmass          #+#    #+#             */
/*   Updated: 2025/04/21 19:19:42 by hnemmass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>

// typedef struct s_env {
// 	char *name;
// 	char *value;
// 	struct s_env *next;
// } t_env;

// // Include all your functions above here...

// int	ft_strcmp(char *s1, char *s2)
// {
// 	size_t	i;

// 	i = 0;
// 	if (!s1)
// 		return (1);
// 	while (s1[i] || s2[i])
// 	{
// 		if (s1[i] != s2[i])
// 			return (s1[i] - s2[i]);
// 		i++;
// 	}
// 	return (0);
// }

// // Helper to create a new env node
// t_env *new_node(char *name, char *value) {
// 	t_env *node = malloc(sizeof(t_env));
// 	if (!node) return NULL;
// 	node->name = strdup(name);
// 	node->value = value ? strdup(value) : NULL;
// 	node->next = NULL;
// 	return node;
// }

// // Helper to append a node to a list
// void append_node(t_env **head, t_env *new) {
// 	if (!*head)
// 		*head = new;
// 	else {
// 		t_env *curr = *head;
// 		while (curr->next)
// 			curr = curr->next;
// 		curr->next = new;
// 	}
// }

// // Free the original env list (not the copy)
// void free_env(t_env *env) {
// 	t_env *tmp;
// 	while (env) {
// 		tmp = env;
// 		env = env->next;
// 		free(tmp->name);
// 		if (tmp->value)
// 			free(tmp->value);
// 		free(tmp);
// 	}
// }

// char	*ft_strdup(char *s)
// {
// 	char	*d_s;
// 	char	*d;

// 	d_s = (char *)malloc(strlen(s) + 1);
// 	if (!d_s)
// 		return (NULL);
// 	d = d_s;
// 	while (*s)
// 	{
// 		*d_s = *s;
// 		d_s++;
// 		s++;
// 	}
// 	*d_s = '\0';
// 	return (d);
// }

void	free_copy(t_env *copy)
{
	t_env	*current;

	while(copy)
	{
		current = copy;
		copy = copy->next;
		free(current->name);
		if (current->value)
			free(current->value);
		free(current);
	}
}

int	check_if_sorted(t_env *env)
{
	t_env	*current;
	int		i;

	current = env;
	while (current && current->next)
	{
		i = 0;
		while (current->name[i] == current->next->name[i] && (current->name[i] || current->next->name[i]))
			i++;
		if (current->name[i] > current->next->name[i])
			return (1);
		current = current->next;
	}
	return (0);
}
t_env	*create_copy(t_env *env)
{
	t_env	*current;
	t_env	*copy;
	t_env	*new_node;
	t_env	*last;

	copy = NULL;
	last = NULL;
	current = env;
	while (current)
	{
		new_node = malloc(sizeof(t_env));
		if (!new_node)
			return (NULL);
		new_node->name = ft_strdup(current->name);
		if (current->value)
			new_node->value = ft_strdup(current->value);
		else
			new_node->value = NULL;
		new_node->next = NULL;
		if (!copy)
			copy = new_node;
		else
			last->next = new_node;
		last = new_node;
		current = current->next;
	}
	return (copy);
}

void sort_copy(t_env *copy)
{
	t_env	*current;
	char	*tmp_name;
	char	*tmp_value;
	int		i;
	
	if (!copy || !check_if_sorted(copy))
		return ;
	current = copy;
	while (current && current->next)
	{
		i = 0;
		while (current->name[i] == current->next->name[i] && (current->name[i] || current->next->name[i]))
			i++;
		if (current->name[i] > current->next->name[i])
		{
			tmp_name = current->name;
			tmp_value = current->value;
			current->name = current->next->name;
			current->value = current->next->value;
			current->next->name = tmp_name;
			current->next->value = tmp_value;
		}
		current = current->next;
	}
	if (check_if_sorted(copy))
		sort_copy(copy);
}

void sort_and_display(t_env *env)
{
	t_env	*copy;
	t_env	*current;
	
	copy = create_copy(env);
	if (!copy)
		return ;
	sort_copy(copy);
	current = copy;
	while (current)
	{
		if (current->value)
			printf("declare -x %s=\"%s\"\n", current->name, current->value);
		else
			printf("declare -x %s\n", current->name);
		current = current->next;
	}
	free_copy(copy);
}

int check_valid_input(char *cmd)
{
    int i = 0;
    
    if (!cmd[0])
        return (1);
    if ((cmd[0] != '_') && 
        (cmd[0] < 'a' || cmd[0] > 'z') && 
        (cmd[0] < 'A' || cmd[0] > 'Z'))
        return (1);
    while (cmd[i] && cmd[i] != '=' && !(cmd[i] == '+' && cmd[i+1] == '='))
    {
        if (i > 0 && (cmd[i] < 'a' || cmd[i] > 'z') && 
            (cmd[i] < 'A' || cmd[i] > 'Z') && 
            (cmd[i] < '0' || cmd[i] > '9') && 
            cmd[i] != '_')
            return (1);
        i++;
    }
    if (i == 0)
        return (1);
    if (cmd[i] == '\0')
        return (2);
    else if (cmd[i] == '=')
        return (3);
    else if (cmd[i] == '+' && cmd[i+1] == '=')
        return (4);
    return (0);
}

void	export(char **cmd, t_env *env)
{
	int	i;

	i = 0;
	if(!cmd[1])
		sort_and_display(env);
	while (cmd[i])
	{
		if(check_valid_input(cmd[i]))
		{
			remove_quotes(cmd[i]);
			add_back(env);
		}
		i++;
	}
}

// int main(void)
// {
// 	t_env *env = NULL;

// 	// Edge case 1: Variable with NULL value
// 	append_node(&env, new_node("EMPTY", NULL)); // should print: declare -x EMPTY

// 	// Edge case 2: Variable names with underscores and ASCII order sensitivity
// 	append_node(&env, new_node("__X", "secret2"));
// 		append_node(&env, new_node("_X", "secret"));
// 	append_node(&env, new_node("__x", "secret2"));
// 	append_node(&env, new_node("_x", "public"));

// 	// Edge case 3: Names with numeric suffixes
// 	append_node(&env, new_node("X1", "val1"));
// 	append_node(&env, new_node("X10", "val10"));
// 	append_node(&env, new_node("X2", "val2"));
// 	append_node(&env, new_node("X", "===\"kmlk\""));

// 	// Edge case 4: Very long name and value
// 	append_node(&env, new_node("LONGVARXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX", 
// 		"LONGVALUEYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY"));

// 	// Edge case 5: Variable with only one character
// 	append_node(&env, new_node("Q", "quick"));

// 	// Edge case 6: Mixed casing (should sort by ASCII, so uppercase before lowercase)
// 	append_node(&env, new_node("a", "lowercase"));
// 	append_node(&env, new_node("A", "uppercase"));

// 	char *cmd[] = {"export", NULL}; // simulate export with no args

// 	export(cmd, env);

// 	free_env(env);
// 	return 0;
// }
