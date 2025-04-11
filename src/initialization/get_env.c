/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhajbi <yhajbi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:36:53 by yhajbi            #+#    #+#             */
/*   Updated: 2025/04/11 15:54:03 by yhajbi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

//static int	count_env_size(char **env);
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
		node->name = get_env_name(env[i]);
		node->value = get_env_value(env[i]);
		node->next = malloc(sizeof(t_env));
		node = node->next;
		i++;
	}
	node->next = NULL;
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