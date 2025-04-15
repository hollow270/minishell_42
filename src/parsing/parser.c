/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhajbi <yhajbi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 18:16:34 by yhajbi            #+#    #+#             */
/*   Updated: 2025/04/14 19:46:32 by yhajbi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static t_cmd	**process_tokens(t_token *s_tokens);
static	void	cmdadd_back(t_cmd **head, char **argv, t_tokens_type type);
static char	**make_argv(t_token *cur, t_token *prv);
static int	count_args(t_token *s_tokens);
static int	identify_cmd(char *cmd);

t_cmd	**parse(t_token *s_tokens)
{
	t_cmd	**s_cmd;

	s_cmd = process_tokens(s_tokens);
	return (s_cmd);
	/*t_cmd	*s_cmd;
	t_token	*node;
	t_token	*prv;

	node = s_tokens;
	prv = s_tokens;
	while (node)
	{
		if (node->type == TOKEN_WORD)
			cmdadd_back(s_cmd, node->value);
	}
	return (s_cmd);*/
}

static t_cmd	**process_tokens(t_token *s_tokens)
{
	t_cmd	**s_cmd;
	t_token	*node;
	t_token	*prv;

	s_cmd = malloc(sizeof(t_cmd *));
	*s_cmd = NULL;
	node = s_tokens;
	prv = s_tokens;
	while (node)
	{
		if (node->type == TOKEN_HDOC)
			cmdadd_back(s_cmd, make_argv(node, prv), node->type);
		else if (prv->type == TOKEN_HDOC && node->type == TOKEN_WORD)
			cmdadd_back(s_cmd, make_argv(node, prv), TOKEN_EOF);
		else if (node->type == TOKEN_WORD && prv->type != TOKEN_WORD)
			cmdadd_back(s_cmd, make_argv(node, prv), TOKEN_CMD);
		prv = node;
		node = node->next;
	}
	return (s_cmd);
}

static	void	cmdadd_back(t_cmd **head, char **argv, t_tokens_type type)
{
	t_cmd	*new;
	t_cmd	*node;

	new = (t_cmd *)malloc(sizeof(t_cmd));
	if (!new)
		return ;
	node = *head;
	if (!node)
	{
		node = new;
		new->argv = argv;
		new->type = type;
		new->is_builtin = identify_cmd(argv[0]);
		node->next = NULL;
		*head = new;
		return ;
	}
	while (node->next)
		node = node->next;
	node->next = new;
	new->argv = argv;
	new->type = type;
	new->is_builtin = identify_cmd(argv[0]);
	new->next = NULL;
}

static char	**make_argv(t_token *cur, t_token *prv)
{
	char	**argv;
	t_token	*node;
	int		cmd_sz;
	int		i;

	node = cur;
	if (node->type == TOKEN_HDOC)
		return (*argv = ft_strdup("here_doc"), argv);
	if (node->type == TOKEN_WORD && prv->type == TOKEN_HDOC)
		return (*argv = ft_strdup(node->value), argv);
	cmd_sz = count_args(node);
	i = 0;
	argv = malloc((cmd_sz + 1) * sizeof(char *));
	if (!argv)
		return (NULL);
	while (node && node->type == TOKEN_WORD)
	{
		argv[i] = ft_strdup(node->value);
		node = node->next;
		i++;
	}
	argv[i] = NULL;
	return (argv);
}

static int	count_args(t_token *s_tokens)
{
	int		cnt;
	t_token	*node;

	cnt = 0;
	node = s_tokens;
	while (node && node->type == TOKEN_WORD)
	{
		cnt++;
		node = node->next;
	}
	return (cnt);
}

static int	identify_cmd(char *cmd)
{
	if (ft_strcmp(cmd, "echo") == 0)
		return (1);
	if (ft_strcmp(cmd, "cd") == 0)
		return (1);
	if (ft_strcmp(cmd, "pwd") == 0)
		return (1);
	if (ft_strcmp(cmd, "export") == 0)
		return (1);
	if (ft_strcmp(cmd, "unset") == 0)
		return (1);
	if (ft_strcmp(cmd, "env") == 0)
		return (1);
	if (ft_strcmp(cmd, "exit") == 0)
		return (1);
	return (0);
}