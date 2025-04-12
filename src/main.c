/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhajbi <yhajbi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 17:16:24 by yhajbi            #+#    #+#             */
/*   Updated: 2025/04/12 20:38:39 by yhajbi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static t_minishell	*init_minishell(char **env, t_status *e_status);
static t_status	minishell(t_minishell **s_minishell);
static void	print_env(t_env *env);

/*			---------		MAIN		--------			*/

int main(int arc, char **argv, char **env)
{
	t_minishell	*s_minishell;
	t_status	e_status;

	(void)arc;
	(void)argv;
	s_minishell = init_minishell(env, &e_status);
	if (!s_minishell)
		return (e_status);
	while (true)
	{
		e_status = minishell(&s_minishell);
		if (e_status == STATUS_EXIT_CMD)
			break ;
	}
	return (free_minishell(s_minishell), STATUS_SUCCESS);
}

static t_minishell	*init_minishell(char **env, t_status *e_status)
{
	t_minishell	*s_minishell;

	s_minishell = (t_minishell *)malloc(sizeof(t_minishell));
	if (!s_minishell)
		return (*e_status = STATUS_MALLOC_FAIL, NULL);
	s_minishell->s_env = get_env(env);
	if (!s_minishell->s_env)
		return (*e_status = STATUS_MALLOC_FAIL, NULL);
	s_minishell->cwd = get_env_value(s_minishell->s_env, "PWD");
	if (!s_minishell->cwd)
		return (*e_status = STATUS_MALLOC_FAIL, free_env(s_minishell->s_env), NULL);
	s_minishell->stdfd[0] = dup(STDIN_FILENO);
	s_minishell->stdfd[1] = dup(STDOUT_FILENO);
	return (s_minishell);
}

static t_status	minishell(t_minishell **s_minishell)
{
	t_minishell	*s_ms;

	s_ms = *s_minishell;
	s_ms->cmdline = readline(PROMPT);
	if (!s_ms->cmdline)
		return (STATUS_FAILURE);
	if (ft_strcmp(s_ms->cmdline, "exit") == 0)
	{
		rl_clear_history();
		free(s_ms->cmdline);
		return (STATUS_EXIT_CMD);
	}
	add_history(s_ms->cmdline);
	printf("%s\n", s_ms->cmdline);
	free(s_ms->cmdline);
	return (STATUS_SUCCESS);
}

static void	print_env(t_env *env)
{
	t_env	*node;

	node = env;
	while (node)
	{
		printf("%s=%s\n", node->name, node->value);
		node = node->next;
	}
}