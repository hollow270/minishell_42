/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhajbi <yhajbi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 17:03:11 by yhajbi            #+#    #+#             */
/*   Updated: 2025/04/13 14:39:25 by yhajbi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

typedef struct	s_env
{
	char			*name;
	char			*value;
	struct s_env	*next;
}				t_env;

typedef struct	s_minishell
{
	char	*cmdline;
	t_env	*s_env;
	char	*cwd;
	t_token	*s_tokens;
	int		stdfd[2];
}			t_minishell;

typedef enum	s_status
{
	STATUS_SUCCESS,
	STATUS_FAILURE,
	STATUS_MALLOC_FAIL,
	STATUS_EXIT_CMD
}				t_status;

#endif