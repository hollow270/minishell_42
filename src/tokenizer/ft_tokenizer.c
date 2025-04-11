/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tokenizer.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnemmass <hnemmass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 14:29:49 by hnemmass          #+#    #+#             */
/*   Updated: 2025/04/11 16:36:04 by hnemmass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	ft_isspace(char c)
{
	if ((c >= 9 && c <= 13) || c == 32)
		return (1);
	return (0);
}

t_token	*ft_tokenizer(char *line)
{
	char	*word;
	int		i;
	int		j;

	i = 0;
	while (line[i])
	{
		if (ft_isspace(line[i]))
			i++;
		while (line[i] != '|' && line[i] != '<' && line[i] != '>')
		{
			if (line[i] == '<' && line[i + 1] == '<')
				i++;
			if (line[i] == '>' && line[i + 1] == '>')
				i++;
			word[j] = line[i];
		}
	}
}