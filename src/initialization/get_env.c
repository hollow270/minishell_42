/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhajbi <yhajbi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:36:53 by yhajbi            #+#    #+#             */
/*   Updated: 2025/04/11 18:44:41 by yhajbi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static char	*get_env_name(char *s);
static char	*get_env_value(char *s);

t_env	*get_env(char **env)
{
	int		i;
	t_env	*ret;
	t_env	*node;

	i = 0;
	ret = malloc(sizeof(t_env));
	if (!ret)
		return (NULL);
	node = ret;
	while (env[i])
	{
		node->name = get_env_name(env[i]); // check if null
		node->value = get_env_value(env[i]);
		if (env[i + 1])
		{
			node->next = malloc(sizeof(t_env));
			if (!(node->next))
				return (NULL);
			node = node->next;
		}
		else
			node->next = NULL;
		i++;
	}
	return (ret);
}

static char	*get_env_name(char *s)
{
	int		i;
	char	*ret;

	i = 0;
	while (s[i] && s[i] != '=')
		i++;
	s[i] = '\0';
	ret = ft_strdup(s);
	s[i] = '=';
	return (ret);
}

static char	*get_env_value(char *s)
{
	int		i;
	char	*ret;

	i = 0;
	while (s[i] && s[i] != '=')
		i++;
	i++;
	ret = ft_strdup(s + i);
	return (ret);
}