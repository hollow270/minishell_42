/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhajbi <yhajbi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 17:16:24 by yhajbi            #+#    #+#             */
/*   Updated: 2025/04/10 16:49:33 by yhajbi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static t_status	init_minishell(t_minishell **s_minishell, char **env);

int main(int arc, char **argv, char **env)
{
	t_minishell	s_minishell;
	t_status	e_status;

	(void)arc;
	(void)argv;
	e_status = init_minishell(&s_minishell, env);
	if (e_status)
	{
		free_minishell(&s_minishell);
		exit(EXIT_FAILURE);
	}
	return (0);
}

static t_status	init_minishell(t_minishell **s_minishell, char **env)
{
	char	*buff;

	*s_minishell = malloc(size_of(t_minishell));
	if (!*s_minishell)
		return (STATUS_MALLOC_FAIL);
	(*s_minishell)->prompt = ft_strdup(PROMPT);
	(*s_minishell)->env = get_env(env);
	if (!(*s_minishell)->env)
		return (STATUS_MALLOC_FAIL);
	(*s_minishell)->cwd = getcwd(buff, 10);				// change later
}