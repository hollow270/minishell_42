/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhajbi <yhajbi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:45:45 by yhajbi            #+#    #+#             */
/*   Updated: 2025/04/20 16:52:18 by yhajbi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENVIRONMENT_H
# define ENVIRONMENT_H

# include "structs.h"

t_env	*get_env(char **env);
char	*get_env_value(t_env *s_env, char *name);
char	*get_env_value2(t_env *s_env, char *name);

#endif