/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnemmass <hnemmass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 17:16:39 by yhajbi            #+#    #+#             */
/*   Updated: 2025/04/11 16:34:27 by hnemmass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_h

#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <curses.h>
#include <term.h>
#include <unistd.h>
#include <stdlib.h>

# define PROMPT "minishell> "

typedef enum	e_tokens_type
{
	TOKEN_WORD,		/*CMD OR ARG*/ 
	TOKEN_PIPE,		/* | */
	TOKEN_RED_IN,	/* < */
	TOKEN_RED_OUT,	/* > */
	TOKEN_HDOC,		/* << */
	TOKEN_APPEND,	/* >> */
}		t_tokens_type;

typedef struct	s_token
{
	char			*value;
	t_tokens_type	type;
	struct s_token	*next;
}		t_token;

#endif