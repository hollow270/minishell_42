/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execute_helper.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnemmass <hnemmass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 17:13:37 by hnemmass          #+#    #+#             */
/*   Updated: 2025/06/19 18:16:43 by hnemmass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/execution.h"

char	*ft_strjoin_free(char *s1, char *s2)
{
	char	*result;

	result = ft_strjoin(s1, s2);
	free(s1);
	return (result);
}

char	*ft_substr_2(char const *s, unsigned int start, size_t len)
{
	char	*result;
	size_t	i;
	size_t	s_len;

	if (!s)
		return (NULL);
	s_len = strlen(s);
	if (start >= s_len)
		return (ft_strdup(""));
	if (len > s_len - start)
		len = s_len - start;
	result = (char *)malloc(sizeof(char) * (len + 1));
	if (!result)
		return (NULL);
	i = 0;
	while (i < len && s[start + i])
	{
		result[i] = s[start + i];
		i++;
	}
	result[i] = '\0';
	return (result);
}

void	free_split_2(char **split)
{
	int	i;

	i = 0;
	if (!split)
		return ;
	while (split[i])
		free(split[i++]);
	free(split);
}

static void	free_allocated_strings(char **s1, int count)
{
	while (count > 0)
	{
		free(s1[count - 1]);
		count--;
	}
	free(s1);
}

static int	count_w(char const *s, char c)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (s[i])
	{
		while (s[i] == c)
			i++;
		if (s[i] != c && s[i])
		{
			j++;
			while (s[i] != c && s[i])
				i++;
		}
	}
	return (j);
}

static char	**allocate_a_cpy(char const *s, char **s1, int start, int end)
{
	*s1 = ft_substr_2(s, start, (end - start));
	if (!*s1)
		return (NULL);
	return (s1);
}

static char	**count_a_cpy(char const *s, char **s1, char c)
{
	int	i;
	int	start;
	int	index;

	index = 0;
	i = 0;
	while (s[i])
	{
		while (s[i] == c)
			i++;
		start = i;
		while (s[i] != c && s[i])
			i++;
		if (start < i)
		{
			if (allocate_a_cpy(s, s1 + index, start, i) == NULL)
			{
				free_allocated_strings(s1, index);
				return (NULL);
			}
			index++;
		}
	}
	s1[index] = NULL;
	return (s1);
}

char	**ft_split_2(char const *s, char c)
{
	size_t	total_count;
	char	**s1;

	if (!s)
		return (NULL);
	total_count = count_w(s, c) + 1;
	s1 = malloc(total_count * sizeof(char *));
	if (!s1)
		return (NULL);
	if (count_a_cpy(s, s1, c) == NULL)
		return (NULL);
	return (s1);
}

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

#include <errno.h>

static void check_cwd(char **cmd, char **env_array)
{
    char *temp;
    
    temp = getcwd(NULL, 0);
    if (!temp)
        return;
    temp = ft_strjoin(temp, "/");
    temp = ft_strjoin_free(temp, cmd[0]);
    if (access(temp, F_OK) == 0)
    {
        execve(temp, cmd, env_array);
        perror(cmd[0]);
        free(temp);
        exit(126);
    }
    perror(cmd[0]);
    free(temp);
    exit(127);
}

static void find_cmd_path(char **path, char **cmd, char **env_array, t_minishell *mini)
{
    char *temp;
    int i;

    i = -1;
    while (path[++i])
    {
        temp = ft_strjoin(path[i], "/");
        temp = ft_strjoin_free(temp, cmd[0]);
        
        if (access(temp, F_OK) == 0)
        {
            execve(temp, cmd, env_array);
            perror(cmd[0]);
            free(temp);
            exit(126);
        }
        free(temp);
    }
    dup2(mini->stdfd[1], STDOUT_FILENO);
    printf("%s: command not found\n", cmd[0]);
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

static void	print_exec_error(char *path, int code)
{
	if (code == 127)
	{
		ft_putstr_fd("bash: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
	}
	else if (code == 126)
	{
		ft_putstr_fd("bash: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": Permission denied\n", 2);
	}
}

static void	free_exec_argv(char **argv)
{
	int	i;

	i = 0;
	while (argv[i])
		i++;
	free(argv);
}

static void	exec_script_with_bash(char *path, char **argv, char **envp)
{
	char	**new_argv;
	int		i;
	int		argc;

	argc = 0;
	while (argv[argc])
		argc++;
	new_argv = malloc(sizeof(char *) * (argc + 2));
	if (!new_argv)
		exit(126);
	new_argv[0] = "/bin/bash";
	new_argv[1] = path;
	i = 1;
	while (i <= argc)
	{
		new_argv[i + 1] = argv[i];
		i++;
	}
	execve("/bin/bash", new_argv, envp);
	free_exec_argv(new_argv);
}

void	exec_path_command(char *path, char **argv, char **envp)
{
	if (access(path, F_OK) != 0)
	{
		print_exec_error(path, 127);
		exit(127);
	}
	if (access(path, X_OK) != 0)
	{
		print_exec_error(path, 126);
		exit(126);
	}
	execve(path, argv, envp);
	if (errno == ENOEXEC)
		exec_script_with_bash(path, argv, envp);
	perror("bash");
	exit(126);
}


void	exec_cmd(char **cmd, t_env *env, t_minishell *mini)
{
	char	*tmp_path;
	char	**path;
	char	**env_array;
	char	*sh;

	if (!cmd || !(*cmd))
		exit(0);
	env_array = env_to_array(env);
	if (!env_array)
	{
		printf("Error: problem in the environment\n");
		return ;
	}
	tmp_path = make_path(env);
	if (!tmp_path)
	{
		check_cwd(cmd, env_array);
		return (printf("Error: PATH not found in environment\n"), exit(127));
	}
	path = ft_split_2(tmp_path, ':');
	if (!path)
	{
		printf("Error: PATH not found\n");
		return ;
	}
	if (cmd[0][0] == '/' || (cmd[0][0] == '.' && cmd[0][1] == '/'))
		exec_path_command(cmd[0], cmd, env_array);
	else
		find_cmd_path(path, cmd, env_array, mini);
	free_split_2(path);
	exit(127);
}
