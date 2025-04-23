/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhajbi <yhajbi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 21:14:33 by yhajbi            #+#    #+#             */
/*   Updated: 2025/04/23 21:35:09 by yhajbi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static char		*split_dquotes(char *s);
static char		*remove_quotes(char *s);
static int		count_quotes(char *s);
static int	is_separate_sdquotes(char *s);
static t_quotes	leading_quotes(char *s);

void	handle_quotes(t_token *s_tokens)
{
	t_token	*node;
	char	*old_value;

	node = s_tokens;
	old_value = NULL;
	while (node)
	{
		old_value = node->value;
		node->value = remove_quotes(node->value);
		if (old_value != NULL)
			free(old_value);
		node = node->next;
	}
}

static char	*remove_quotes(char *s)
{
	int		i;
	int		j;
	char	cur_quote;
	char	*ret;

	i = 0;
	cur_quote = '\0';
	ret = malloc(sizeof(char) * ft_strlen(s) - count_quotes(s) + 1);
	if (!ret)
		return (NULL);
	while (s[i])
	{
		if ((s[i] == '\'' || s[i] == '\"') && cur_quote == '\0')
			cur_quote = s[i];
		else if (s[i] == cur_quote && s[i])
			cur_quote = '\0';
		else
		{
			ret[j] = s[i];
			j++;
		}
		i++;
	}
	return (ret);
}

static int	count_quotes(char *s)
{
	int	i;
	int	count;
	int	cur_quote;

	i = 0;
	count = 0;
	cur_quote = '\0';
	while (s[i])
	{
		if ((s[i] == '\'' || s[i] == '\"') && cur_quote == '\0')
		{
			cur_quote = s[i];
			count++;
		}
		else if (s[i] == cur_quote)
		{
			cur_quote = '\0';
			count++;
		}
		i++;
	}
	return (count);
}

/*void	handle_quotes(t_token *s_tokens)
{
    t_token	*node;
	char	*old_value;

	node = s_tokens;
	old_value = NULL;
	while (node)
	{
		old_value = node->value;
		node->value = split_dquotes(node->value);
		if (old_value != NULL)
			free(old_value);
		node = node->next;
	}
}

static char	*split_dquotes(char *s)
{
	int		i;
	char	**split;
	char	*ret;
	char	*old;

	if (is_separate_sdquotes(s) == 1)
		split = ft_split(s, "\"\'");
	else if (leading_quotes(s) == DOUBLE_QUOTES)
		split = ft_split(s, "\"");
	else if (leading_quotes(s) == SINGLE_QUOTES)
		split = ft_split(s, "\'");
	else if (leading_quotes(s) == NO_QUOTES)
		return (s);
	if (!split)
		return (NULL);
		i = 0;
	ret = NULL;
	old = NULL;
	while (split[i])
	{
		old = ret;
		ret = ft_strconcat(ret, split[i++]);
		if (old != NULL)
			free(old);
	}
	return (ret);
}

static int	is_separate_sdquotes(char *s)
{
	int	i;
	int	dquotes;
	int	squotes;

	i = 0;
	dquotes = 0;
	squotes = 0;
	while (s[i])
	{
		if (s[i] == '\"')
			dquotes++;
		else if (s[i] == '\'')
			squotes++;
		i++;
	}
	if (dquotes % 2 == 0 && squotes % 2 == 0)
		return (1);
	return (0);
}

static t_quotes	leading_quotes(char *s)
{
	int	i;
	int	first_dquotes;
	int	first_squotes;

	i = 0;
	first_dquotes = -1;
	first_squotes = -1;
	if (!s || !*s)
		return (NO_QUOTES);
	while (s[i])
	{
		if (s[i] == '\"' && first_dquotes == -1)
			first_dquotes = i;
		else if (s[i] == '\'' && first_squotes == -1)
			first_squotes = i;
		i++;
	}
	if (first_dquotes != -1 && (first_squotes == -1 || first_dquotes < first_squotes))
		return (DOUBLE_QUOTES);
	else if (first_squotes != -1)
		return (SINGLE_QUOTES);
	return (NO_QUOTES);
}*/
