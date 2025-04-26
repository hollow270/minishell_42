/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhajbi <yhajbi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 18:06:02 by yhajbi            #+#    #+#             */
/*   Updated: 2025/04/26 21:01:09 by yhajbi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static char			*expand_split(char *s, t_env *s_env);
static char			*expand(char *ret, char *var_name, t_env *s_env);
static int			count_dollar(char *s);
static void			copy_value(char *ret2, char *value, int i, char *ret);
static int			lookup_var(t_env *s_env, char *name);
static char			*find_env_value(char *s, t_env *s_env);

void	expand_variables(t_token *s_tokens, t_env *s_env)
{
	t_token	*node;
	char	*old_value;

	node = s_tokens;
	old_value = node->value;
	while (node)
	{
		old_value = node->value;
		if (has_var(node->value))
		{
			node->value = expand_split(node->value, s_env);
			//printf("[%s]\n", node->value);
			free(old_value);
		}
		node = node->next;
	}
}

static char	*expand_split(char *s, t_env *s_env)
{
	int		i;
	char	**split;
	char	*ret;
	char	*old;

	i = 0;
	split = ft_split(s, "$");
	ret = NULL;
	old = NULL;
	int	j = 0;
	while (split[j])
		printf("[%s]\n", split[j++]);
	while (split[i])
	{
		if (lookup_var(s_env, split[i]) == 1)
			ret = expand(ret, split[i], s_env);
		else
		{
			old = ret;
			ret = ft_strconcat(ret, split[i]);
			if (old != NULL)
				free(old);
		}
		i++;
	}
	return (ret);
}

static char	*expand(char *ret, char *var_name, t_env *s_env)
{
	int		i;
	int		d_cnt;
	char	*ret2;
	char	*value;

	d_cnt = count_dollar(var_name);
	if (!ret)
		ret2 = malloc(sizeof(char) * (ft_strlen(get_env_value2(s_env, var_name))
			+ 1 - d_cnt));
	else
		ret2 = malloc(sizeof(char) * (ft_strlen(ret)
			+ ft_strlen(get_env_value2(s_env, var_name)) + 1 - d_cnt));
	if (!ret2)
		return (NULL);
	i = 0;
	value = get_env_value2(s_env, var_name);
	if (ret)
	{
		while (ret[i])
		{
			ret2[i] = ret[i];
			i++;
		}
	}
	copy_value(ret2, value, i, ret);
	free(ret);
	return (ret2);
}

static int	count_dollar(char *s)
{
	int	i;

	i = 0;
	while (s[i] && s[i] == '$')
		i++;
	return (i);
}

static void	copy_value(char *ret2, char *value, int i, char *ret)
{
	int	j;

	j = 0;
	if (!value)
	{
		ret2[i] = '\0';
		return ;
	}
	while (value[j] && value[j] == '$')
		j++;
	while (value[j])
	{
		ret2[i] = value[j];
		i++;
		j++;
	}
	j = 0;
	while (ret[j])
	{
		if (ret[j] == ' ')
			ret2[i] = ret[j];
		i++;
		j++;
	}
	ret2[i] = '\0';
}

static char	*find_env_value(char *s, t_env *s_env)
{
	t_env	*node;

	node = s_env;
	while (node)
	{
		if (ft_strcmp2(s, node->name))
			return (node->value);
		node->next;
	}
	return (NULL);
}

static int	lookup_var(t_env *s_env, char *name)
{
	int		i;
	t_env	*node;

	i = 0;
	node = s_env;
	while (name[i] == '$')
		i++;
	while (node)
	{
		if (ft_strcmp2(node->name, name + i) == 0)
			return (1);
		node = node->next;
	}
	return (0);
}