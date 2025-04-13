/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhajbi <yhajbi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 16:43:21 by hnemmass          #+#    #+#             */
/*   Updated: 2025/04/13 14:56:46 by yhajbi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENIZER_H
# define TOKENIZER_H

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

t_token	*ft_tokenizer(char *line);
void	ft_free_tokens(t_token *head);

#endif