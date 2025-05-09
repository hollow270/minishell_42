/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnemmass <hnemmass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 13:43:43 by hnemmass          #+#    #+#             */
/*   Updated: 2025/05/08 15:40:05 by hnemmass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/execution.h"

int	ft_pwd(t_minishell *mini)
{
	char	*buffer;

	buffer = getcwd(NULL, 0);
	if (!buffer)
		return (printf("%s\n", mini->cwd), 0);
	else
	{
		printf("%s\n", buffer);
		free (buffer);
		return (0);
	}
}