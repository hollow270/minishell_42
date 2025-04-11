/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhajbi <yhajbi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 17:03:11 by yhajbi            #+#    #+#             */
/*   Updated: 2025/04/10 16:33:37 by yhajbi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

typedef struct	s_minishell
{
	char	*prompt;
	char	*cmdline;
	t_env	**env;
	char	*cwd;
}				t_minishell;

typedef struct	s_env
{
	char			*name;
	char			*value;
	struct s_env	*next;
}				t_env;

typedef enum	s_status
{
	STATUS_SUCCESS,
	STATUS_FAILURE,
	STATUS_MALLOC_FAIL
}				t_status;

#endif