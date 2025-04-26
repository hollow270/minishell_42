/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhajbi <yhajbi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 17:08:40 by yhajbi            #+#    #+#             */
/*   Updated: 2025/04/24 18:04:32 by yhajbi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void			process_tokens(t_token *s_tokens);
t_cmd				*parse_tokens(t_token *s_tokens);
static t_cmd		*create_cmd(void);
static void			add_cmd(t_cmd **head, t_cmd *new);
static t_redirect	*create_redirect(void);
static void			add_redirect(t_redirect **head, t_redirect *new);
static void			add_arg(t_cmd *s_cmd, char *arg);
static void			free_argv(char **argv);
static char			*expand_env_var(t_env *s_env, char *var);

t_status	parse_command_line(t_minishell *s_minishell)
{
	if (!s_minishell || !s_minishell->s_tokens)
		return (printf("minishell: syntax error\n"), STATUS_FAILURE);
	if (check_syntax(s_minishell->s_tokens) == STATUS_SYNTAX_ERR)
		return (printf("minishell: syntax error\n"), STATUS_SYNTAX_ERR);
	process_tokens(s_minishell->s_tokens);
	handle_quotes(s_minishell->s_tokens);
	expand_variables(s_minishell->s_tokens, s_minishell->s_env);
	/*s_minishell->s_cmd = parse_tokens(s_minishell->s_tokens);
	if (!s_minishell->s_cmd)
		return (STATUS_FAILURE);*/
	return (STATUS_SUCCESS);
}

static void		process_tokens(t_token *s_tokens)
{
	char	c;
	t_token	*node;
	t_token	*prv;

	node = s_tokens;
	prv = s_tokens;
	while (node)
	{
		c = *(node->value);
		if (c == '$')
			node->type = TOKEN_VAR;
		else if (node->type == TOKEN_WORD && (prv->type == TOKEN_RED_IN || prv->type == TOKEN_RED_OUT))
			node->type = TOKEN_FILE;
		else if (node->type == TOKEN_WORD && prv->type == TOKEN_HDOC)
			node->type = TOKEN_EOF;
		else if (node->type == TOKEN_WORD && prv->type == TOKEN_APPEND)
			node->type = TOKEN_FILE;
		else if (node->type == TOKEN_WORD && (prv->type != TOKEN_WORD
			&& prv->type != TOKEN_CMD && prv->type != TOKEN_ARG))
			node->type = TOKEN_CMD;
		else if (node->type == TOKEN_WORD && (prv->type == TOKEN_CMD || prv->type == TOKEN_ARG))
			node->type = TOKEN_ARG;
		prv = node;
		node = node->next;
	}
}

int	has_var(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == '$')
			return (1);
		i++;
	}
	return (0);
}

t_cmd	*parse_tokens(t_token *s_tokens)
{
	t_cmd	*s_cmd;
	t_cmd	*curr_cmd;
	t_token	*curr_token;

	s_cmd = malloc(sizeof(s_cmd));
	if (!s_cmd)
		return (NULL);
	curr_cmd = s_cmd;
	curr_token = s_tokens;
	while (curr_token)
	{
		if (curr_token->type == TOKEN_PIPE)
		{
			add_cmd(&s_cmd, curr_cmd);
			curr_cmd = create_cmd();
		}
		else if (curr_token->type == TOKEN_RED_IN || curr_token->type == TOKEN_RED_OUT
			|| curr_token->type == TOKEN_HDOC || curr_token->type == TOKEN_APPEND)
		{
			if (curr_token->next->type == TOKEN_VAR)
				return (s_cmd);
		}
	}
	return (s_cmd);
}

static t_cmd	*create_cmd(void)
{
	t_cmd	*new;

	new = malloc(sizeof(t_cmd));
	if (!new)
		return (NULL);
	new->argv = NULL;
	new->is_builtin = 0;
	new->s_redirect = NULL;
	new->next = NULL;
	return (new);
}

static void	add_cmd(t_cmd **head, t_cmd *new)
{
	t_cmd	*node;

	node = *head;
	if (!node)
	{
		*head = new;
		return ;
	}
	while (node->next)
		node = node->next;
	node->next = new;
}

static t_redirect	*create_redirect(void)
{
	t_redirect	*new;

	new = malloc(sizeof(t_redirect));
	if (!new)
		return (NULL);
	new->type = 0;
	new->file = NULL;
	new->next = NULL;
	return (new);
}

static void	add_redirect(t_redirect **head, t_redirect *new)
{
	t_redirect	*node;

	node = *head;
	if (!*head)
	{
		*head = new;
		return ;
	}
	while (node->next)
		node = node->next;
	node->next = new;
}

static void	add_arg(t_cmd *s_cmd, char *arg)
{
	char	**new_argv;
	int		i;
	int		j;

	i = 0;
	while (s_cmd->argv[i])
		i++;
	new_argv = malloc(sizeof(char *) * (i + 2));
	if (!new_argv)
		return ;
	j = 0;
	while (s_cmd->argv[i])
	{
		new_argv[j] = ft_strdup(s_cmd->argv[j]);
		j++;
	}
	new_argv[j] = ft_strdup(arg);
	new_argv[j + 1] = NULL;
	if (s_cmd->argv)
		free_argv(s_cmd->argv);
	s_cmd->argv = new_argv;
}

static void	free_argv(char **argv)
{
	int	i;

	i = 0;
	while (argv[i])
		free(argv[i]);
	free(argv);
}

static char	*expand_env_var(t_env *s_env, char *var)
{
	int		i;
	t_env	*node;

	i = 0;
	node = s_env;
	while (var[i] && var[i] == '$')
		i++;
	while (node)
	{
		if (ft_strcmp(var + i, node->name) == 0)
			return (ft_strdup(node->value));
		node = node->next;
	}
	return (ft_strdup(""));
}