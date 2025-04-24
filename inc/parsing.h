/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhajbi <yhajbi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 19:32:07 by yhajbi            #+#    #+#             */
/*   Updated: 2025/04/24 11:41:03 by yhajbi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include "structs.h"

t_status	parse_command_line(t_minishell *s_minishell);
void		handle_quotes(t_token *s_tokens);
int			has_var(char *s);
t_status	check_syntax(t_token *s_tokens);

#endif