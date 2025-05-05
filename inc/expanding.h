/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhajbi <yhajbi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 18:08:14 by yhajbi            #+#    #+#             */
/*   Updated: 2025/04/20 18:12:01 by yhajbi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANDING_H
# define EXPANDING_H

#include "minishell.h"

void	expand_variables(t_token *s_tokens, t_env *s_env);

#endif