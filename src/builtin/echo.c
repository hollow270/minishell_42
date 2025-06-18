/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnemmass <hnemmass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 17:02:42 by hnemmass          #+#    #+#             */
/*   Updated: 2025/06/18 14:24:40 by hnemmass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/execution.h"

int	compare_argument(char *str)
{
	int	i;

	i = 0;
	while(str[i])
	{
		if (str[i] == '-')
		{
			i++;
			while (str[i] && str[i] == 'n')
				i++;
			if (!str[i])
				return (0);
			return (1);
		}
		return (1);
	}
}

int	ft_echo(char **cmd)
{
	char	**current;
	int		i;

	i = 1;
	if (!cmd[1])
		return (printf("\n"), 0);
	if ((compare_argument(cmd[i]) == 0 && !cmd[2]))
		return (0);
	current = cmd;
	if (compare_argument(current[i]) == 0)
	{
		i++;
		while (current[i] && compare_argument(current[i]) == 0)
			i++;
		if (!current[i])
			return (0);
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
