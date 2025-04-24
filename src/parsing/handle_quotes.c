/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhajbi <yhajbi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 21:14:33 by yhajbi            #+#    #+#             */
/*   Updated: 2025/04/24 11:06:29 by yhajbi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int		has_quotes(char *s);
static char		*remove_quotes(char *s);
static int		count_quotes(char *s);

void	handle_quotes(t_token *s_tokens)
{
	t_token	*node;
	char	*old_value;

	node = s_tokens;
	old_value = NULL;
	while (node)
	{
		old_value = node->value;
		if (has_quotes(node->value))
		{
			node->value = remove_quotes(node->value);
			if (old_value != NULL)
				free(old_value);
		}
		node = node->next;
	}
}

static int	has_quotes(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == '\'' || s[i] == '\"')
			return (1);
		i++;
	}
	return (0);
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
