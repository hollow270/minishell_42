/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnemmass <hnemmass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 17:02:42 by hnemmass          #+#    #+#             */
/*   Updated: 2025/04/19 17:59:11 by hnemmass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	echo(char **cmd)
{
	char	**current;
	int		i;

	i = 1;
	current = cmd;
	if (ft_strcmp(current[i], "-n") == 0)
	{
		i++;
		while (ft_strcmp(current[i], "-n") == 0)
			i++;
		while (current[i + 1])
		{
			printf("%s ", current[i]);
			i++;
		}
		printf("%s", current[i]);
		return (0);
	}
	else
	{
		while (current[i + 1])
		{
			printf("%s ", current[i]);
			i++;
		}
		printf("%s\n", current[i]);
		return (0);
	}
	return (1);
}
