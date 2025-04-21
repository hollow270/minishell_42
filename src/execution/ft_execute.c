/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execute.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnemmass <hnemmass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 16:45:12 by hnemmass          #+#    #+#             */
/*   Updated: 2025/04/19 16:57:54 by hnemmass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	**env_to_array(t_env *env)
{
	t_env	*tmp_env;
	char	**env_array;
	char	*temp;
	int		count;

	count = 0;
	tmp_env = env;
	while(tmp_env)
	{
		count++;
		tmp_env = tmp_env->next;
	}
	env_array = malloc(sizeof(char *) * (count + 1));
	if (env_array)
		return (NULL);
	count = 0;
	tmp_env = env;
	while (tmp_env)
	{
		temp = ft_strjoin(tmp_env->name, "=");
		env_array[count] = ft_strjoin(temp, tmp_env->value);
		free(temp);
		count++;
		tmp_env = tmp_env->next;
	}
	env_array[count] = NULL;
	return (env_array);
}

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
	ft_putstr_fd("Error: Command not found\n", 2);
	exit(127);
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
		return (printf("Error: problem in the environment\n"), 1);
	tmp_path = make_path(env);
	if (!tmp_path)
		return (printf("Error: PATH not found in environment\n"), exit(127));
	path = ft_split(tmp_path, ':');
	if (!path)
		handle_error(6);
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

pid_t	do_pipe(char **cmd, t_env *env, int flag, int redirection)
{
	pid_t	pid;
	int		p_fd[2];

	if (pipe(p_fd) == -1)
		handle_error(4);
	pid = fork();
	if (pid == -1)
		handle_error(5);
	if (pid == 0)
	{
		close(p_fd[0]);
		if (redirection != 2)
		{
			dup2(p_fd[1], 1);
			close(p_fd[1]);
		}
		if (flag == 0)
			exec_cmd(cmd, env);
		if (flag == 1)
			exec_builtin(cmd, env);
	}
	close(p_fd[1]);
	dup2(p_fd[0], 0);
	close(p_fd[0]);
	return (pid);
}

void	ft_execute(t_cmd *data, t_minishell *env)
{
	int		redirection;
	t_cmd	*current;

	current = data;
	while (current)
	{
		redirection = 0;
		if (current->s_redirect)
		{
			while (current->s_redirect)
			{
				redirection = apply_redirections(current->s_redirect);
				if (redirection == 1)
					return (1);
				current->s_redirect = current->s_redirect->next;
			}
		}
		if (current->argv && current->is_builtin == 0)
			do_pipe(current->argv, env->s_env, 0, redirection);
		if (current->argv && current->is_builtin == 1)
			do_pipe(current->argv, env->s_env, 1, redirection);
		current = current->next;
	}
}
