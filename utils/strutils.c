/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strutils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhajbi <yhajbi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 17:38:43 by yhajbi            #+#    #+#             */
/*   Updated: 2025/04/11 17:32:35 by yhajbi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	ft_strlen(const char *s)
{
	int	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

char	*ft_strdup(const char *s)
{
	char	*d_s;
	char	*d;

	d_s = (char *)malloc(ft_strlen(s) + 1);
	if (!d_s)
		return (NULL);
	d = d_s;
	while (*s)
	{
		*d_s = *s;
		d_s++;
		s++;
	}
	*d_s = '\0';
	return (d);
}

char	*ft_strndup(const char *s, int n)
{	
	char	*d_s;
	char	*d;

	d_s = (char *)malloc(ft_strlen(s) + 1);
	if (!d_s)
		return (NULL);
	d = d_s;
	while (*s && n)
	{
		*d_s = *s;
		d_s++;
		s++;
		n--;
	}
	*d_s = '\0';
	return (d);
}