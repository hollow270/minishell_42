/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_minishell.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhajbi <yhajbi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 14:12:57 by yhajbi            #+#    #+#             */
/*   Updated: 2025/04/11 18:58:06 by yhajbi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static void	free_env(t_minishell *s_minishell);

void	free_minishell(t_minishell *s_minishell)
{
	free(s_minishell->prompt);
	//free((*s_minishell)->cmdline);			// do later
	free_env(s_minishell);
	//free((*s_minishell)->cwd);				// do later
}

static void	free_env(t_minishell *s_minishell)
{
	t_env	*p;
	t_env	*node;
	t_env	*cup;

	p = s_minishell->env;
	node = p;
	while(node)
	{
		cup = node->next;
//		printf("%s=%s\n", node->name, node->value);
		free(node->name);
		free(node->value);
		free(node);
		node = cup;
	}
	//free(*p);
	//p  = NULL;
}