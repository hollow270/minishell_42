/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhajbi <yhajbi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 17:16:24 by yhajbi            #+#    #+#             */
/*   Updated: 2025/04/11 15:54:16 by yhajbi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static t_status	init_minishell(t_minishell **s_minishell, char **env);
static void	print_env(t_env *env);

/*			---------		HEAD		--------			*/

int main(int arc, char **argv, char **env)
{
	t_minishell	*s_minishell;
	t_status	e_status;

	(void)arc;
	(void)argv;
	e_status = init_minishell(&s_minishell, env);
	if (e_status)
	{
		free_minishell(&s_minishell);
		exit(EXIT_FAILURE);
	}
	return (STATUS_SUCCESS);
}

static t_status	init_minishell(t_minishell **s_minishell, char **env)
{
	//char	*buff;
	t_env	*ret_env;

	*s_minishell = malloc(sizeof(s_minishell));
	if (!*s_minishell)
		return (STATUS_MALLOC_FAIL);
	(*s_minishell)->prompt = ft_strdup(PROMPT);
	ret_env = get_env(env);
	(*s_minishell)->env = &ret_env;
	if (!(*s_minishell)->env)
		return (STATUS_MALLOC_FAIL);
	print_env(ret_env);
	//(*s_minishell)->cwd = getcwd(buff, 10);				// change later
	return (STATUS_SUCCESS);
}

static void	print_env(t_env *env)
{
	t_env	*node;

	node = env;
	while (node)
	{
		printf("name = %s\n", node->name);
		printf("value = %s\n", node->value);
		node = node->next;
	}
}