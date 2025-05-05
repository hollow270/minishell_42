/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execute_helper.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnemmass <hnemmass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 17:13:37 by hnemmass          #+#    #+#             */
/*   Updated: 2025/04/30 15:36:50 by hnemmass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/execution.h"

// void	ft_putchar_fd(char c, int fd)
// {
// 	if (fd < 0)
// 		return ;
// 	write (fd, &c, 1);
// }

// void	ft_putstr_fd(char *s, int fd)
// {
// 	if (s == NULL || fd < 0)
// 		return ;
// 	while (*s != '\0')
// 	{
// 		ft_putchar_fd(*s, fd);
// 		s++;
// 	}
// }

static void	find_cmd_path(char **path, char **cmd, char **env_array)
{
	char	*temp;
	int		i;

	i = -1;
	while (path[++i])
	{
		temp = ft_strjoin(path[i], "/");
		temp = ft_strjoin_free(temp, cmd[0]);
		if (access(temp, F_OK | X_OK) == 0)
		{
			execve(temp, cmd, env_array);
			free(temp);
			exit(127);
		}
		free(temp);
	}
	ft_putstr_fd(cmd[0], 2);
	ft_putstr_fd(": command not found\n", 2);
	_exit(127);
}
char	*make_path(t_env *env)
{
	t_env	*current;

	current = env;
	while (current)
	{
		if(ft_strcmp(current->name, "PATH") == 0)
			return (current->value);
		current = current->next;
	}
	return(NULL);
}

void	exec_cmd(char **cmd, t_env *env)
{
	char	*tmp_path;
	char	**path;
	char	**env_array;

	env_array = env_to_array(env);
	if (!env_array)
	{
		printf("Error: problem in the environment\n");
		return ;
	}
	tmp_path = make_path(env);
	if (!tmp_path)
		return (printf("Error: PATH not found in environment\n"), exit(127));
	path = ft_split(tmp_path, ':');
	if (!path)
	{
		printf("Error: PATH not found\n");
		return ;
	}
	if (cmd[0][0] == '/' || (cmd[0][0] == '.' && cmd[0][1] == '/'))
	{
		if (access(cmd[0], F_OK | X_OK) == 0)
			execve(cmd[0], cmd, env_array);
		printf("%s: command not found\n", cmd[0]);
	}
	else
		find_cmd_path(path, cmd, env_array);
	free_split(path);
	exit(127);
}
