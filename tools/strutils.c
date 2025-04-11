/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strutils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhajbi <yhajbi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 17:38:43 by yhajbi            #+#    #+#             */
/*   Updated: 2025/04/09 17:42:15 by yhajbi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	ft_strlen(char *s)
{
	int	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

char	*ft_strdup(char *s)
{
	int		i;
	int		sz;
	char	*ret;

	i = 0;
	sz = ft_strlen(s);
	ret = malloc((sz + 1) * sizeof(char));
	while (s[i])
	{
		ret[i] = s[i];
		i++;
	}
	s[i] = '\0';
	return (ret);
}