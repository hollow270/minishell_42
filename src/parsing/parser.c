/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhajbi <yhajbi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 18:16:34 by yhajbi            #+#    #+#             */
/*   Updated: 2025/04/15 16:57:35 by yhajbi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void		process_tokens(t_token *s_tokens);
static t_cmd	*parse_processed_tokens(t_token *s_tokens);
static	void	cmdadd_back(t_cmd **head, char **argv, t_tokens_type type);
static char		**make_argv(t_token *cur, t_token *prv);
static char	**make_argv_helper(char *s);
static int		count_args(t_token *s_tokens);
static int		identify_cmd(char *cmd);

t_cmd	*parse(t_token *s_tokens)
{
	t_cmd	*s_cmd;

	process_tokens(s_tokens);
	s_cmd = parse_processed_tokens(s_tokens);
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

static void		process_tokens(t_token *s_tokens)
{
	t_token	*node;
	t_token	*prv;

	node = s_tokens;
	prv = s_tokens;
	while (node)
	{
		if (node->type == TOKEN_WORD && (prv->type == TOKEN_RED_IN || prv->type == TOKEN_RED_OUT))
			node->type = TOKEN_FILE;
		prv = node;
		node = node->next;
	}
}

static t_cmd	*parse_processed_tokens(t_token *s_tokens)
{
	t_cmd	*s_cmd;
	t_token	*node;
	t_token	*prv;

	s_cmd = NULL;
	//s_cmd = (t_cmd *)malloc(sizeof(t_cmd));
	node = s_tokens;
	prv = s_tokens;
	while (node)
	{
		if (node->type == TOKEN_HDOC)
			cmdadd_back(&s_cmd, make_argv(node, prv), node->type);
		else if (prv->type == TOKEN_HDOC && node->type == TOKEN_WORD)
			cmdadd_back(&s_cmd, make_argv(node, prv), TOKEN_EOF);
		else if (node->type == TOKEN_WORD && prv->type != TOKEN_WORD)
			cmdadd_back(&s_cmd, make_argv(node, prv), TOKEN_CMD);
		else if (node->type == TOKEN_FILE)
			cmdadd_back(&s_cmd, make_argv(node, prv), TOKEN_FILE);
		prv = node;
		node = node->next;
	}
	return (s_cmd);
}

static	void	cmdadd_back(t_cmd **head, char **argv, t_tokens_type type)
{
	    t_cmd *new;
    t_cmd *node;

    // Allocate memory for the new command node
    new = malloc(sizeof(t_cmd));
    if (!new)
        return;

    // Initialize the new node's fields
    new->argv = argv;
    new->type = type;
    new->is_builtin = identify_cmd(argv[0]);
    new->next = NULL;

    // If the list is empty, set the new node as the head
    if (*head == NULL)
    {
        *head = new;
        return;
    }

    // Otherwise, find the last node and append the new node
    node = *head;
    while (node->next)
        node = node->next;
    node->next = new;

	/*t_cmd	*new;
	t_cmd	*node;

	new = malloc(sizeof(t_cmd));
	if (!new)
		return ;
	node = *head;
	if (!node)
	{
		//node = new;
		*head = new;
		new->argv = argv;
		new->type = type;
		new->is_builtin = identify_cmd(argv[0]);
		node->next = NULL;
		return ;
	}
	while (node->next)
		node = node->next;
	node->next = new;
	new->argv = argv;
	new->type = type;
	new->is_builtin = identify_cmd(argv[0]);
	new->next = NULL;*/
}

static char	**make_argv(t_token *cur, t_token *prv)
{
	char	**argv;
	t_token	*node;
	int		cmd_sz;
	int		i;

	node = cur;
	if (node->type == TOKEN_HDOC)
		return (make_argv_helper("here_doc"));
		//return (*argv = ft_strdup("here_doc"), argv);
	if (node->type == TOKEN_WORD && prv->type == TOKEN_HDOC)
		return (make_argv_helper(node->value));
		//return (*argv = ft_strdup(node->value), argv);
	if (node->type == TOKEN_FILE)
		return (make_argv_helper(node->value));
		//return (*argv = ft_strdup(node->value), argv);
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

static char	**make_argv_helper(char *s)
{
	char	**ret;

	ret = malloc(2 * sizeof(char *));
	ret[0] = ft_strdup(s);
	ret[1] = NULL;
	return (ret);
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