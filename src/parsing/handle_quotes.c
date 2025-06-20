/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhajbi <yhajbi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 21:14:33 by yhajbi            #+#    #+#             */
/*   Updated: 2025/06/21 21:26:14 by yhajbi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*#include "../../inc/minishell.h"

static t_token			*handle_export_expanding(t_token **head, t_token *node, t_token *prv, t_env *s_env, int exit_status);
static char				*add_space_front(char **s);
static char				*add_space_back(char **s);
static int				splitlen(char **split);
static t_export_parse	*expand_export_fragments(char **split, t_env *s_env, int exit_status);
static void				addlst_export_parse(t_export_parse **head, char	*new_frag);
static void				split_after_expand(t_export_parse **head);
static void				find_and_join_equal(t_export_parse **head);
static void				join_three_nodes(t_export_parse *prv, t_export_parse *node, t_export_parse *next);
static void				addlst_export_back(t_export_parse **head, t_export_parse **rest);
static void				freelst_export(t_export_parse *node);
static void				split_and_insert_expanded(t_export_parse **head, t_export_parse *node);
static void				insert_expanded_in_tokens(t_token **start, t_export_parse *head, t_token *original_node);
static t_token			*find_last_token(t_export_parse **exp_head, t_token **head);
static char				*remove_quotes(char *s, t_env *s_env, int exit_status);
static int				is_word(t_tokens_type type);
static t_substring		*split_quotes(char *s);
static void				add_substring(t_substring **head, char *s, char quote);
static t_quotes			identify_quotes(char quote);
static int				count_substrings(char *s);
static char				*join_substrings(t_substring *head);
static char				*leave_outer_quotes(char *s);
static void				split_and_insert_tokens(t_token **head, t_token *node);
static void 			change_tabs(char *line);
static void				free_split(char **split);
static char				**split_preserve_spaces(char *s);
static void				free_substrings(t_substring *head);

// Add this function to your file
static void remove_empty_node(t_token **head, t_token **current, t_token **previous)
{
    t_token *to_delete = *current;
    
    if (!to_delete)
        return;
    
    // If we're removing the first node (head)
    if (*previous == *current) {
        *head = (*current)->next;
        *current = *head;
        *previous = *head;
    } else {
        (*previous)->next = (*current)->next;
        *current = (*current)->next;
    }
    
    // Free the node
    if (to_delete->value)
        free(to_delete->value);
    free(to_delete);
}

// Modified handle_quotes function - replace the relevant section
void handle_quotes(t_token **s_tokens, t_env *s_env, int exit_status)
{
    t_token *node;
    t_token *prv;
    char *old_value;
    int export_flag;
    int should_free_old_value;

    node = *s_tokens;
    prv = *s_tokens;
    old_value = NULL;
    export_flag = 0;
    
    while (node)
    {
        old_value = node->value;
        should_free_old_value = 0;
        
        if (ft_strcmp(node->value, "export") == 0)
            export_flag = 1;
        else if (node->type == TOKEN_PIPE)
            export_flag = 0;
            
        if (export_flag == 1)
        {
            t_token *next_after_expansion = handle_export_expanding(s_tokens, node, prv, s_env, exit_status);
            if (next_after_expansion)
            {
                prv = next_after_expansion;
                node = next_after_expansion->next;
                continue;
            }
        }
        
        if (has_quotes(node->value) && node->type != TOKEN_EOF)
        {
            node->value = remove_quotes(node->value, s_env, exit_status);
            should_free_old_value = 1;
        }
        else if (has_quotes(node->value) && node->type == TOKEN_EOF)
        {
            node->value = leave_outer_quotes(node->value);
            should_free_old_value = 1;
        }
        else if (!has_quotes(node->value) && is_word(node->type))
        {
            node->value = scan_string(node->value, s_env, exit_status);
            if (has_var(old_value) && !ft_strchr(old_value, '='))
                split_and_insert_tokens(s_tokens, node);
            should_free_old_value = 1;
        }
        
        // Check if the node should be removed (empty after variable expansion)
        if (ft_strcmp(node->value, "") == 0 && has_var(old_value) && has_quotes(old_value) == 0
			&& prv->type != TOKEN_RED_OUT && prv->type != TOKEN_RED_IN && prv->type != TOKEN_APPEND)
        {
            if (should_free_old_value && old_value != NULL)
                free(old_value);
            remove_empty_node(s_tokens, &node, &prv);
            continue; // Skip the rest of the loop iteration
        }
        
        // Free old_value only if it was replaced and we didn't remove the node
        if (should_free_old_value && old_value != NULL)
            free(old_value);
            
        if (export_flag == 0)
            prv = node;
        node = node->next;
    }
}

static t_token	*handle_export_expanding(t_token **head, t_token *node, t_token *prv, t_env *s_env, int exit_status)
{
	t_export_parse	*exp_head;
	char			**split;

	if (ft_strchr(node->value, '$') == NULL)
		return (NULL);
	split = ft_split(node->value, "=");
	if (node->value[0] == ' ')
		split[0] = add_space_front(&(node->value));
	else if (node->value[ft_strlen(node->value) - 1] == ' ')
		split[splitlen(split) - 1] = add_space_back(&(node->value)); 
	if (!split || !split[0] || ft_strchr(split[0], '$') == NULL)
	{
		if (split)
			free_split(split);
		return (NULL);
	}
	exp_head = expand_export_fragments(split, s_env, exit_status);
	free_split(split);
	if (!exp_head)
		return (NULL);
	split_after_expand(&exp_head);
	find_and_join_equal(&exp_head);
	insert_expanded_in_tokens(&prv, exp_head, node);
	return (find_last_token(&exp_head, head));
}

static char	*add_space_front(char **s)
{
	int		i;
	char	*ret;
	int		sz;

	sz = ft_strlen(*s);
	ret = malloc(sizeof(char) * (sz + 2));
	i = 0;
	ret[0] = ' ';
	while (*(s[i]))
	{
		ret[i + 1] = *s[i];
		i++;
	}
	free(*(s));
	return (ret);
}

static char	*add_space_back(char **s)
{
	int		i;
	char	*ret;
	int		sz;

	sz = ft_strlen(*s);
	ret = malloc(sizeof(char) * (sz + 2));
	i = 0;
	while (*(s[i]))
	{
		ret[i] = *s[i];
		i++;
	}
	ret[sz + 1] = ' ';
	free(*(s));
	return (ret);
}

static int	splitlen(char **split)
{
	int	count;

	count = 0;
	while (split[count])
		count++;
	return (count);
}

static t_export_parse	*expand_export_fragments(char **split, t_env *s_env, int exit_status)
{
	t_export_parse	*head;
	t_export_parse	*node;
	char			*old_value;
	int				i;

	head = NULL;
	i = 0;
	while (split[i])
	{
		addlst_export_parse(&head, split[i++]);
		if (split[i])
			addlst_export_parse(&head, "=");
	}
	node = head;
	while (node)
	{
		old_value = node->fragment;
		node->fragment = scan_string(node->fragment, s_env, exit_status);
		if (ft_strcmp(old_value, node->fragment) != 0)
			node->type = WAS_EXPANDED;
		if (old_value != NULL)
			free(old_value);
		node = node->next;
	}
	return (head);
}

static void	addlst_export_parse(t_export_parse **head, char	*new_frag)
{
	t_export_parse	*node;
	t_export_parse	*new;

	node = *head;
	if (!node)
	{
		*head = malloc(sizeof(t_export_parse));
		if (!(*head))
			return ;
		(*head)->fragment = ft_strdup(new_frag);
		(*head)->type = WAS_NOT_EXPANDED;
		(*head)->next = NULL;
		return ;
	}
	while (node->next)
		node = node->next;
	new = malloc(sizeof(t_export_parse));
	if (!new)
		return ;
	new->fragment = ft_strdup(new_frag);
	new->type = WAS_NOT_EXPANDED;
	new->next = NULL;
	node->next = new;
}

static void	split_after_expand(t_export_parse **head)
{
	t_export_parse	*node;
	t_export_parse	*next;

	node = *head;
	while (node)
	{
		next = node->next;
		if (node->type == WAS_EXPANDED && ft_strchr(node->fragment, ' '))
			split_and_insert_expanded(head, node);
		node = next;
	}
}

static void	find_and_join_equal(t_export_parse **head)
{
	t_export_parse	*node;
	t_export_parse	*next;
	t_export_parse	*prv;

	if (!head || !*head)
		return;
	
	node = *head;
	prv = NULL;
	
	while (node && node->next && node->next->next)
	{
		next = node->next;
		if (ft_strcmp("=", next->fragment) == 0)
		{
			join_three_nodes(node, next, next->next);
			if (prv)
				prv->next = node;
			else
				*head = node;
		}
		else
		{
			prv = node;
			node = node->next;
		}
	}
}

static void	join_three_nodes(t_export_parse *prv, t_export_parse *node, t_export_parse *next)
{
	char	*new_frag;
	int		sz;

	if (!prv || !node || !next)
		return;
	
	sz = ft_strlen(prv->fragment) + ft_strlen(node->fragment) + ft_strlen(next->fragment);
	new_frag = malloc(sizeof(char) * (sz + 1));
	if (!new_frag)
		return;
	
	ft_strlcpy(new_frag, prv->fragment, sz + 1);
	ft_strlcat(new_frag, node->fragment, sz + 1);
	ft_strlcat(new_frag, next->fragment, sz + 1);
	
	free(prv->fragment);
	prv->fragment = new_frag;
	prv->next = next->next;
	
	freelst_export(node);
	freelst_export(next);
}

static void	addlst_export_back(t_export_parse **head, t_export_parse **rest)
{
	t_export_parse	*node;

	if (!head || !*head || !rest || !*rest)
		return;
	
	node = *head;
	while (node->next)
		node = node->next;
	node->next = *rest;
}

static void	freelst_export(t_export_parse *node)
{
	if (!node)
		return;
	if (node->fragment)
		free(node->fragment);
	free(node);
}

static void	split_and_insert_expanded(t_export_parse **head, t_export_parse *node)
{
	char			**split_words;
	t_export_parse	*new_token;
	t_export_parse	*rest;
	int				i;

	//split_words = split_preserve_spaces(node->fragment);
	split_words = ft_split(node->fragment, " ");
	if (!split_words || !split_words[0])
	{
		if (split_words)
			free_split(split_words);
		return;
	}
	
	free(node->fragment);
	node->fragment = ft_strdup(split_words[0]);
	if (!node->fragment)
		return;
	node->type = WAS_NOT_EXPANDED;
	rest = node->next;
	
	i = 1;
	t_export_parse *current = node;
	while (split_words[i])
	{
		new_token = malloc(sizeof(t_export_parse));
		if (!new_token)
			break;
		new_token->fragment = ft_strdup(split_words[i]);
		new_token->type = WAS_EXPANDED;
		new_token->next = NULL;
		current->next = new_token;
		current = new_token;
		i++;
	}
	current->next = rest;
	free_split(split_words);
}

static void	insert_expanded_in_tokens(t_token **start, t_export_parse *head, t_token *original_node)
{
	t_token			*node;
	t_export_parse	*exp_node;
	t_token			*new_token;
	t_token			*rest;

	if (!start || !*start || !head || !original_node)
		return;
	
	node = *start;
	exp_node = head;
	rest = original_node->next;
	
	// Free the original node's value and replace with first fragment
	free(original_node->value);
	original_node->value = ft_strdup(exp_node->fragment);
	exp_node = exp_node->next;
	
	t_token *current = original_node;
	while (exp_node)
	{
		new_token = malloc(sizeof(t_token));
		if (!new_token)
			return;
		new_token->value = ft_strdup(exp_node->fragment);
		new_token->type = TOKEN_ARG;
		new_token->next = NULL;
		current->next = new_token;
		current = new_token;
		exp_node = exp_node->next;
	}
	current->next = rest;
	
	// Free the export parse list
	while (head)
	{
		t_export_parse *temp = head;
		head = head->next;
		freelst_export(temp);
	}
}

static t_token	*find_last_token(t_export_parse **exp_head, t_token **head)
{
	t_token			*token;
	t_export_parse	*export;

	if (!exp_head || !*exp_head || !head || !*head)
		return (NULL);
	
	token = *head;
	export = *exp_head;
	while (export->next)
		export = export->next;
	while (token)
	{
		if (ft_strcmp(token->value, export->fragment) == 0)
			return (token);
		token = token->next;
	}
	return (NULL);
}

int	has_quotes(char *s)
{
	int	i;

	if (!s)
		return (0);
	i = 0;
	while (s[i])
	{
		if (s[i] == '\'' || s[i] == '\"')
			return (1);
		i++;
	}
	return (0);
}

static int	is_word(t_tokens_type type)
{
	return (type == TOKEN_CMD || type == TOKEN_ARG || type == TOKEN_VAR
		|| type == TOKEN_WORD || type == TOKEN_FILE || type == TOKEN_APPEND
		|| type == TOKEN_FILE);
}

static char	*remove_quotes(char *s, t_env *s_env, int exit_status)
{
	t_substring	*list;
	char		*ret;

	if (!s)
		return (NULL);
	
	list = split_quotes(s);
	if (!list)
		return (NULL);
	
	expand_variables(&list, s_env, exit_status);
	ret = join_substrings(list);
	free_substrings(list);
	return (ret);
}

static t_substring	*split_quotes(char *s)
{
	t_substring	*list;
	int			i;
	int			start;
	char		cur_quote;

	if (!s)
		return (NULL);
	
	list = NULL;
	i = 0;
	start = 0;
	cur_quote = '\0';
	while (s[i])
	{
		if ((s[i] == '\'' || s[i] == '\"') && cur_quote == '\0')
		{
			if (start != i) // Save text before quote
				add_substring(&list, ft_substr(s, start, i - start), 0);
			cur_quote = s[i];
			start = i + 1;
		}
		else if (s[i] == cur_quote)
		{
			add_substring(&list, ft_substr(s, start, i - start), cur_quote);
			cur_quote = '\0';
			start = i + 1;
		}
		i++;
	}
	if (start < i) // Save leftover text after the loop
		add_substring(&list, ft_substr(s, start, i - start), 0);
	return (list);
}

static void	add_substring(t_substring **head, char *s, char quote)
{
	t_substring	*new;
	t_substring	*node;

	new = malloc(sizeof(t_substring));
	if (!new)
		return ;
	new->str = s;
	new->type = identify_quotes(quote);
	new->next = NULL;
	if (!*head)
	{
		*head = new;
		return ;
	}
	node = *head;
	while (node->next)
		node = node->next;
	node->next = new;
}

static t_quotes	identify_quotes(char quote)
{
	if (quote == '\'')
		return (SINGLE_QUOTED);
	else if (quote == '\"')
		return (DOUBLE_QUOTED);
	return (UNQUOTED);
}

static int	count_substrings(char *s)
{
	int		i;
	int		count;
	char	cur_quote;

	if (!s)
		return (0);
	i = 0;
	count = 0;
	cur_quote = '\0';
	while (s[i])
	{
		if ((s[i] == '\'' || s[i] == '\"') && cur_quote == '\0')
			cur_quote = s[i];
		else if (s[i] == cur_quote && s[i])
		{
			cur_quote = '\0';
			count++;
		}
		i++;
	}
	return (count);
}

static char	*join_substrings(t_substring *head)
{
	t_substring	*node;
	char		*ret;

	if (!head)
		return (ft_strdup(""));
	
	node = head;
	ret = ft_strdup("");
	if (!ret)
		return (NULL);
	
	while (node)
	{
		char *temp = ft_strconcat(ret, node->str);
		if (!temp)
		{
			free(ret);
			return (NULL);
		}
		free(ret);
		ret = temp;
		node = node->next;
	}
	return (ret);
}

static char	*leave_outer_quotes(char *s)
{
	char	*ret;
	char	quote;
	int		i;

	if (!s)
		return (NULL);
	
	ret = ft_calloc(1, 1);
	quote = '\0';
	i = 0;
	while (s[i])
	{
		if ((s[i] == '\'' || s[i] == '\"') && quote == '\0')
		{
			quote = s[i];
			if (s[i + 1] != quote)
				ret = str_append_char(ret, s[i]);
		}
		else if (s[i] == quote)
		{
			if (s[i - 1] != quote)
				ret = str_append_char(ret, s[i]);
			quote = '\0';
		}
		else
			ret = str_append_char(ret, s[i]);
		i++;
	}
	return (ret);
}

static void	split_and_insert_tokens(t_token **head, t_token *node)
{
	char	**split_words;
	t_token	*new_token;
	t_token	*rest;
	int		i;

	if (!node || !node->value || ft_strcmp(node->value, "") == 0)
		return;
	
	change_tabs(node->value);
	split_words = ft_split(node->value, " ");
	if (!split_words || !split_words[0])
	{
		if (split_words)
			free_split(split_words);
		return;
	}
	
	free(node->value);
	node->value = ft_strdup(split_words[0]);
	rest = node->next;
	
	i = 1;
	t_token *current = node;
	while (split_words[i])
	{
		new_token = malloc(sizeof(t_token));
		if (!new_token)
			break;
		new_token->value = ft_strdup(split_words[i]);
		new_token->type = node->type;
		new_token->next = NULL;
		current->next = new_token;
		current = new_token;
		i++;
	}
	current->next = rest;
	free_split(split_words);
}

static void change_tabs(char *line)
{
    int i;

    if (!line)
        return;
    i = 0;
    while (line[i])
    {
        if (line[i] == '\t')
            line[i] = ' ';
        i++;
    }
}

static void	free_split(char **split)
{
	int	i;

	if (!split)
		return;
	i = 0;
	while (split[i])
		free(split[i++]);
	free(split);
}

static void	free_substrings(t_substring *head)
{
	t_substring *temp;
	
	while (head)
	{
		temp = head;
		head = head->next;
		if (temp->str)
			free(temp->str);
		free(temp);
	}
}

// This function splits while preserving leading/trailing spaces
static char	**split_preserve_spaces(char *s)
{
	char	**result;
	int		count = 0;
	int		i = 0;
	int		start = 0;
	int		in_space = 0;
	
	if (!s)
		return (NULL);
	
	// Count segments (including empty ones for leading/trailing spaces)
	while (s[i])
	{
		if (s[i] == ' ')
		{
			if (!in_space)
			{
				count++;
				in_space = 1;
			}
		}
		else
		{
			if (in_space)
			{
				count++;
				in_space = 0;
			}
		}
		i++;
	}
	if (!in_space && i > 0)
		count++;
	
	result = malloc(sizeof(char *) * (count + 1));
	if (!result)
		return (NULL);
	
	i = 0;
	int result_idx = 0;
	start = 0;
	in_space = 0;
	
	while (s[i])
	{
		if (s[i] == ' ')
		{
			if (!in_space)
			{
				// End of word segment
				result[result_idx++] = ft_substr(s, start, i - start);
				start = i;
				in_space = 1;
			}
		}
		else
		{
			if (in_space)
			{
				// End of space segment
				result[result_idx++] = ft_substr(s, start, i - start);
				start = i;
				in_space = 0;
			}
		}
		i++;
	}
	
	// Add final segment
	if (start < i)
		result[result_idx++] = ft_substr(s, start, i - start);
	
	result[result_idx] = NULL;
	return (result);
}*/

#include "../../inc/minishell.h"

static t_token			*handle_export_expanding(t_token **head, t_token *node, t_token *prv, t_env *s_env, int exit_status);
static char				*add_space_front(char **s);
static char				*add_space_back(char **s);
static int				splitlen(char **split);
static t_export_parse	*expand_export_fragments(char **split, t_env *s_env, int exit_status);
static void				addlst_export_parse(t_export_parse **head, char	*new_frag);
static void				split_after_expand(t_export_parse **head);
static void				find_and_join_equal(t_export_parse **head);
static void				join_three_nodes(t_export_parse *prv, t_export_parse *node, t_export_parse *next);
static void				addlst_export_back(t_export_parse **head, t_export_parse **rest);
static void				freelst_export(t_export_parse *node);
static void				split_and_insert_expanded(t_export_parse **head, t_export_parse *node);
static void				insert_expanded_in_tokens(t_token **start, t_export_parse *head, t_token *original_node);
static t_token			*find_last_token(t_export_parse **exp_head, t_token **head);
static char				*remove_quotes(char *s, t_env *s_env, int exit_status);
static int				is_word(t_tokens_type type);
static t_substring		*split_quotes(char *s);
static void				add_substring(t_substring **head, char *s, char quote);
static t_quotes			identify_quotes(char quote);
static int				count_substrings(char *s);
static char				*join_substrings(t_substring *head);
static char				*leave_outer_quotes(char *s);
static void				split_and_insert_tokens(t_token **head, t_token *node);
static void 			change_tabs(char *line);
static void				free_split(char **split);
static char				**split_preserve_spaces(char *s);
static void				free_substrings(t_substring *head);
static void				free_export_parse_list(t_export_parse *head);

// Add this function to your file
static void remove_empty_node(t_token **head, t_token **current, t_token **previous)
{
    t_token *to_delete = *current;
    
    if (!to_delete)
        return;
    
    // If we're removing the first node (head)
    if (*previous == *current) {
        *head = (*current)->next;
        *current = *head;
        *previous = *head;
    } else {
        (*previous)->next = (*current)->next;
        *current = (*current)->next;
    }
    
    // Free the node
    if (to_delete->value)
        free(to_delete->value);
    free(to_delete);
}

// Modified handle_quotes function - replace the relevant section
void handle_quotes(t_token **s_tokens, t_env *s_env, int exit_status)
{
    t_token *node;
    t_token *prv;
    char *old_value;
    int export_flag;
    int should_free_old_value;

    node = *s_tokens;
    prv = *s_tokens;
    old_value = NULL;
    export_flag = 0;
    
    while (node)
    {
        old_value = node->value;
        should_free_old_value = 0;
        
        if (ft_strcmp(node->value, "export") == 0)
            export_flag = 1;
        else if (node->type == TOKEN_PIPE)
            export_flag = 0;
            
        if (export_flag == 1)
        {
            t_token *next_after_expansion = handle_export_expanding(s_tokens, node, prv, s_env, exit_status);
            if (next_after_expansion)
            {
                prv = next_after_expansion;
                node = next_after_expansion->next;
                continue;
            }
        }
        
        if (has_quotes(node->value) && node->type != TOKEN_EOF)
        {
            node->value = remove_quotes(node->value, s_env, exit_status);
            should_free_old_value = 1;
        }
        else if (has_quotes(node->value) && node->type == TOKEN_EOF)
        {
            node->value = leave_outer_quotes(node->value);
            should_free_old_value = 1;
        }
        else if (!has_quotes(node->value) && is_word(node->type))
        {
            node->value = scan_string(node->value, s_env, exit_status);
            if (has_var(old_value) && !ft_strchr(old_value, '='))
                split_and_insert_tokens(s_tokens, node);
            should_free_old_value = 1;
        }
        
        // Check if the node should be removed (empty after variable expansion)
        if (ft_strcmp(node->value, "") == 0 && has_var(old_value) && has_quotes(old_value) == 0
			&& prv->type != TOKEN_RED_OUT && prv->type != TOKEN_RED_IN && prv->type != TOKEN_APPEND)
        {
            if (should_free_old_value && old_value != NULL)
                free(old_value);
            remove_empty_node(s_tokens, &node, &prv);
            continue; // Skip the rest of the loop iteration
        }
        
        // Free old_value only if it was replaced and we didn't remove the node
        if (should_free_old_value && old_value != NULL)
            free(old_value);
            
        if (export_flag == 0)
            prv = node;
        node = node->next;
    }
}

static t_token	*handle_export_expanding(t_token **head, t_token *node, t_token *prv, t_env *s_env, int exit_status)
{
	t_export_parse	*exp_head;
	char			**split;
	t_token			*result;

	if (ft_strchr(node->value, '$') == NULL)
		return (NULL);
	split = ft_split(node->value, "=");
	if (!split)
		return (NULL);
	
	if (node->value[0] == ' ')
	{
		char *temp = add_space_front(&split[0]);
		if (!temp)
		{
			free_split(split);
			return (NULL);
		}
		free(split[0]);
		split[0] = temp;
	}
	else if (node->value[ft_strlen(node->value) - 1] == ' ')
	{
		char *temp = add_space_back(&split[splitlen(split) - 1]);
		if (!temp)
		{
			free_split(split);
			return (NULL);
		}
		free(split[splitlen(split) - 1]);
		split[splitlen(split) - 1] = temp;
	}
	
	if (!split[0] || ft_strchr(split[0], '$') == NULL)
	{
		free_split(split);
		return (NULL);
	}
	
	exp_head = expand_export_fragments(split, s_env, exit_status);
	free_split(split);
	if (!exp_head)
		return (NULL);
		
	split_after_expand(&exp_head);
	find_and_join_equal(&exp_head);
	insert_expanded_in_tokens(&prv, exp_head, node);
	result = find_last_token(&exp_head, head);
	
	// Free the export parse list
	free_export_parse_list(exp_head);
	
	return (result);
}

// Fixed memory allocation functions
static char	*add_space_front(char **s)
{
	int		i;
	char	*ret;
	int		sz;

	if (!s || !*s)
		return (NULL);
	sz = ft_strlen(*s);
	ret = malloc(sizeof(char) * (sz + 2));
	if (!ret)
		return (NULL);
	i = 0;
	ret[0] = ' ';
	while ((*s)[i])
	{
		ret[i + 1] = (*s)[i];
		i++;
	}
	ret[sz + 1] = '\0';
	return (ret);
}

static char	*add_space_back(char **s)
{
	int		i;
	char	*ret;
	int		sz;

	if (!s || !*s)
		return (NULL);
	sz = ft_strlen(*s);
	ret = malloc(sizeof(char) * (sz + 2));
	if (!ret)
		return (NULL);
	i = 0;
	while ((*s)[i])
	{
		ret[i] = (*s)[i];
		i++;
	}
	ret[sz] = ' ';
	ret[sz + 1] = '\0';
	return (ret);
}

static int	splitlen(char **split)
{
	int	count;

	count = 0;
	while (split && split[count])
		count++;
	return (count);
}

static t_export_parse	*expand_export_fragments(char **split, t_env *s_env, int exit_status)
{
	t_export_parse	*head;
	t_export_parse	*node;
	char			*old_value;
	int				i;

	if (!split)
		return (NULL);
		
	head = NULL;
	i = 0;
	while (split[i])
	{
		addlst_export_parse(&head, split[i++]);
		if (split[i])
			addlst_export_parse(&head, "=");
	}
	
	if (!head)
		return (NULL);
		
	node = head;
	while (node)
	{
		old_value = node->fragment;
		node->fragment = scan_string(node->fragment, s_env, exit_status);
		if (!node->fragment)
		{
			// Handle allocation failure
			free_export_parse_list(head);
			return (NULL);
		}
		if (ft_strcmp(old_value, node->fragment) != 0)
			node->type = WAS_EXPANDED;
		free(old_value);
		node = node->next;
	}
	return (head);
}

static void	addlst_export_parse(t_export_parse **head, char	*new_frag)
{
	t_export_parse	*node;
	t_export_parse	*new;

	if (!head || !new_frag)
		return;
		
	node = *head;
	if (!node)
	{
		*head = malloc(sizeof(t_export_parse));
		if (!(*head))
			return ;
		(*head)->fragment = ft_strdup(new_frag);
		if (!(*head)->fragment)
		{
			free(*head);
			*head = NULL;
			return;
		}
		(*head)->type = WAS_NOT_EXPANDED;
		(*head)->next = NULL;
		return ;
	}
	while (node->next)
		node = node->next;
	new = malloc(sizeof(t_export_parse));
	if (!new)
		return ;
	new->fragment = ft_strdup(new_frag);
	if (!new->fragment)
	{
		free(new);
		return;
	}
	new->type = WAS_NOT_EXPANDED;
	new->next = NULL;
	node->next = new;
}

static void	split_after_expand(t_export_parse **head)
{
	t_export_parse	*node;
	t_export_parse	*next;

	if (!head || !*head)
		return;
		
	node = *head;
	while (node)
	{
		next = node->next;
		if (node->type == WAS_EXPANDED && ft_strchr(node->fragment, ' '))
			split_and_insert_expanded(head, node);
		node = next;
	}
}

static void	find_and_join_equal(t_export_parse **head)
{
	t_export_parse	*node;
	t_export_parse	*next;
	t_export_parse	*prv;

	if (!head || !*head)
		return;
	
	node = *head;
	prv = NULL;
	
	while (node && node->next && node->next->next)
	{
		next = node->next;
		if (ft_strcmp("=", next->fragment) == 0)
		{
			join_three_nodes(node, next, next->next);
			if (prv)
				prv->next = node;
			else
				*head = node;
			// Don't advance prv here since we just modified the list
		}
		else
		{
			prv = node;
			node = node->next;
		}
	}
}

static void	join_three_nodes(t_export_parse *prv, t_export_parse *node, t_export_parse *next)
{
	char	*new_frag;
	int		sz;

	if (!prv || !node || !next)
		return;
	
	sz = ft_strlen(prv->fragment) + ft_strlen(node->fragment) + ft_strlen(next->fragment);
	new_frag = malloc(sizeof(char) * (sz + 1));
	if (!new_frag)
		return;
	
	ft_strlcpy(new_frag, prv->fragment, sz + 1);
	ft_strlcat(new_frag, node->fragment, sz + 1);
	ft_strlcat(new_frag, next->fragment, sz + 1);
	
	free(prv->fragment);
	prv->fragment = new_frag;
	prv->next = next->next;
	
	// Free the middle and end nodes
	free(node->fragment);
	free(node);
	free(next->fragment);
	free(next);
}

static void	addlst_export_back(t_export_parse **head, t_export_parse **rest)
{
	t_export_parse	*node;

	if (!head || !*head || !rest || !*rest)
		return;
	
	node = *head;
	while (node->next)
		node = node->next;
	node->next = *rest;
}

static void	freelst_export(t_export_parse *node)
{
	if (!node)
		return;
	if (node->fragment)
		free(node->fragment);
	free(node);
}

// New function to free entire export parse list
static void	free_export_parse_list(t_export_parse *head)
{
	t_export_parse *temp;
	
	while (head)
	{
		temp = head;
		head = head->next;
		if (temp->fragment)
			free(temp->fragment);
		free(temp);
	}
}

static void	split_and_insert_expanded(t_export_parse **head, t_export_parse *node)
{
	char			**split_words;
	t_export_parse	*new_token;
	t_export_parse	*rest;
	t_export_parse	*current;
	int				i;

	if (!head || !node || !node->fragment)
		return;
		
	split_words = ft_split(node->fragment, " ");
	if (!split_words || !split_words[0])
	{
		if (split_words)
			free_split(split_words);
		return;
	}
	
	free(node->fragment);
	node->fragment = ft_strdup(split_words[0]);
	if (!node->fragment)
	{
		free_split(split_words);
		return;
	}
	node->type = WAS_NOT_EXPANDED;
	rest = node->next;
	
	i = 1;
	current = node;
	while (split_words[i])
	{
		new_token = malloc(sizeof(t_export_parse));
		if (!new_token)
		{
			free_split(split_words);
			return;
		}
		new_token->fragment = ft_strdup(split_words[i]);
		if (!new_token->fragment)
		{
			free(new_token);
			free_split(split_words);
			return;
		}
		new_token->type = WAS_EXPANDED;
		new_token->next = NULL;
		current->next = new_token;
		current = new_token;
		i++;
	}
	current->next = rest;
	free_split(split_words);
}

static void	insert_expanded_in_tokens(t_token **start, t_export_parse *head, t_token *original_node)
{
	t_token			*new_token;
	t_token			*rest;
	t_export_parse	*exp_node;
	t_token			*current;

	if (!start || !*start || !head || !original_node)
		return;
	
	exp_node = head;
	rest = original_node->next;
	
	// Free the original node's value and replace with first fragment
	free(original_node->value);
	original_node->value = ft_strdup(exp_node->fragment);
	if (!original_node->value)
		return;
	exp_node = exp_node->next;
	
	current = original_node;
	while (exp_node)
	{
		new_token = malloc(sizeof(t_token));
		if (!new_token)
			return;
		new_token->value = ft_strdup(exp_node->fragment);
		if (!new_token->value)
		{
			free(new_token);
			return;
		}
		new_token->type = TOKEN_ARG;
		new_token->next = NULL;
		current->next = new_token;
		current = new_token;
		exp_node = exp_node->next;
	}
	current->next = rest;
}

static t_token	*find_last_token(t_export_parse **exp_head, t_token **head)
{
	t_token			*token;
	t_export_parse	*export;

	if (!exp_head || !*exp_head || !head || !*head)
		return (NULL);
	
	token = *head;
	export = *exp_head;
	while (export->next)
		export = export->next;
	while (token)
	{
		if (ft_strcmp(token->value, export->fragment) == 0)
			return (token);
		token = token->next;
	}
	return (NULL);
}

int	has_quotes(char *s)
{
	int	i;

	if (!s)
		return (0);
	i = 0;
	while (s[i])
	{
		if (s[i] == '\'' || s[i] == '\"')
			return (1);
		i++;
	}
	return (0);
}

static int	is_word(t_tokens_type type)
{
	return (type == TOKEN_CMD || type == TOKEN_ARG || type == TOKEN_VAR
		|| type == TOKEN_WORD || type == TOKEN_FILE || type == TOKEN_APPEND
		|| type == TOKEN_FILE);
}

static char	*remove_quotes(char *s, t_env *s_env, int exit_status)
{
	t_substring	*list;
	char		*ret;

	if (!s)
		return (NULL);
	
	list = split_quotes(s);
	if (!list)
		return (ft_strdup(""));
	
	expand_variables(&list, s_env, exit_status);
	ret = join_substrings(list);
	free_substrings(list);
	return (ret);
}

static t_substring	*split_quotes(char *s)
{
	t_substring	*list;
	int			i;
	int			start;
	char		cur_quote;
	char		*substr;

	if (!s)
		return (NULL);
	
	list = NULL;
	i = 0;
	start = 0;
	cur_quote = '\0';
	while (s[i])
	{
		if ((s[i] == '\'' || s[i] == '\"') && cur_quote == '\0')
		{
			if (start != i) // Save text before quote
			{
				substr = ft_substr(s, start, i - start);
				if (substr)
				{
					add_substring(&list, substr, 0);
				}
			}
			cur_quote = s[i];
			start = i + 1;
		}
		else if (s[i] == cur_quote)
		{
			substr = ft_substr(s, start, i - start);
			if (substr)
			{
				add_substring(&list, substr, cur_quote);
			}
			cur_quote = '\0';
			start = i + 1;
		}
		i++;
	}
	if (start < i) // Save leftover text after the loop
	{
		substr = ft_substr(s, start, i - start);
		if (substr)
		{
			add_substring(&list, substr, 0);
		}
	}
	return (list);
}

static void	add_substring(t_substring **head, char *s, char quote)
{
	t_substring	*new;
	t_substring	*node;

	if (!head || !s)
	{
		if (s)
			free(s);
		return;
	}
		
	new = malloc(sizeof(t_substring));
	if (!new)
	{
		free(s);
		return ;
	}
	new->str = s;
	new->type = identify_quotes(quote);
	new->next = NULL;
	if (!*head)
	{
		*head = new;
		return ;
	}
	node = *head;
	while (node->next)
		node = node->next;
	node->next = new;
}

static t_quotes	identify_quotes(char quote)
{
	if (quote == '\'')
		return (SINGLE_QUOTED);
	else if (quote == '\"')
		return (DOUBLE_QUOTED);
	return (UNQUOTED);
}

static int	count_substrings(char *s)
{
	int		i;
	int		count;
	char	cur_quote;

	if (!s)
		return (0);
	i = 0;
	count = 0;
	cur_quote = '\0';
	while (s[i])
	{
		if ((s[i] == '\'' || s[i] == '\"') && cur_quote == '\0')
			cur_quote = s[i];
		else if (s[i] == cur_quote && s[i])
		{
			cur_quote = '\0';
			count++;
		}
		i++;
	}
	return (count);
}

static char	*join_substrings(t_substring *head)
{
	t_substring	*node;
	char		*ret;
	char		*temp;

	if (!head)
		return (ft_strdup(""));
	
	node = head;
	ret = ft_strdup("");
	if (!ret)
		return (NULL);
	
	while (node)
	{
		temp = ft_strconcat(ret, node->str);
		if (!temp)
		{
			free(ret);
			return (NULL);
		}
		free(ret);
		ret = temp;
		node = node->next;
	}
	return (ret);
}

static char	*leave_outer_quotes(char *s)
{
	char	*ret;
	char	quote;
	int		i;

	if (!s)
		return (NULL);
	
	ret = ft_calloc(1, 1);
	if (!ret)
		return (NULL);
		
	quote = '\0';
	i = 0;
	while (s[i])
	{
		if ((s[i] == '\'' || s[i] == '\"') && quote == '\0')
		{
			quote = s[i];
			if (s[i + 1] != quote)
			{
				char *temp = str_append_char(ret, s[i]);
				if (!temp)
				{
					free(ret);
					return (NULL);
				}
				ret = temp;
			}
		}
		else if (s[i] == quote)
		{
			if (s[i - 1] != quote)
			{
				char *temp = str_append_char(ret, s[i]);
				if (!temp)
				{
					free(ret);
					return (NULL);
				}
				ret = temp;
			}
			quote = '\0';
		}
		else
		{
			char *temp = str_append_char(ret, s[i]);
			if (!temp)
			{
				free(ret);
				return (NULL);
			}
			ret = temp;
		}
		i++;
	}
	return (ret);
}

static void	split_and_insert_tokens(t_token **head, t_token *node)
{
	char	**split_words;
	t_token	*new_token;
	t_token	*rest;
	t_token	*current;
	int		i;

	if (!node || !node->value || ft_strcmp(node->value, "") == 0)
		return;
	
	change_tabs(node->value);
	split_words = ft_split(node->value, " ");
	if (!split_words || !split_words[0])
	{
		if (split_words)
			free_split(split_words);
		return;
	}
	
	free(node->value);
	node->value = ft_strdup(split_words[0]);
	if (!node->value)
	{
		free_split(split_words);
		return;
	}
	rest = node->next;
	
	i = 1;
	current = node;
	while (split_words[i])
	{
		new_token = malloc(sizeof(t_token));
		if (!new_token)
		{
			free_split(split_words);
			return;
		}
		new_token->value = ft_strdup(split_words[i]);
		if (!new_token->value)
		{
			free(new_token);
			free_split(split_words);
			return;
		}
		new_token->type = node->type;
		new_token->next = NULL;
		current->next = new_token;
		current = new_token;
		i++;
	}
	current->next = rest;
	free_split(split_words);
}

static void change_tabs(char *line)
{
    int i;

    if (!line)
        return;
    i = 0;
    while (line[i])
    {
        if (line[i] == '\t')
            line[i] = ' ';
        i++;
    }
}

static void	free_split(char **split)
{
	int	i;

	if (!split)
		return;
	i = 0;
	while (split[i])
		free(split[i++]);
	free(split);
}

static void	free_substrings(t_substring *head)
{
	t_substring *temp;
	
	while (head)
	{
		temp = head;
		head = head->next;
		if (temp->str)
			free(temp->str);
		free(temp);
	}
}

// This function splits while preserving leading/trailing spaces
static char	**split_preserve_spaces(char *s)
{
	char	**result;
	int		count = 0;
	int		i = 0;
	int		start = 0;
	int		in_space = 0;
	int		result_idx = 0;
	
	if (!s)
		return (NULL);
	
	// Count segments (including empty ones for leading/trailing spaces)
	while (s[i])
	{
		if (s[i] == ' ')
		{
			if (!in_space)
			{
				count++;
				in_space = 1;
			}
		}
		else
		{
			if (in_space)
			{
				count++;
				in_space = 0;
			}
		}
		i++;
	}
	if (!in_space && i > 0)
		count++;
	
	result = malloc(sizeof(char *) * (count + 1));
	if (!result)
		return (NULL);
	
	i = 0;
	start = 0;
	in_space = 0;
	
	while (s[i])
	{
		if (s[i] == ' ')
		{
			if (!in_space)
			{
				// End of word segment
				result[result_idx] = ft_substr(s, start, i - start);
				if (!result[result_idx])
				{
					// Free already allocated strings on failure
					while (result_idx > 0)
						free(result[--result_idx]);
					free(result);
					return (NULL);
				}
				result_idx++;
				start = i;
				in_space = 1;
			}
		}
		else
		{
			if (in_space)
			{
				// End of space segment
				result[result_idx] = ft_substr(s, start, i - start);
				if (!result[result_idx])
				{
					// Free already allocated strings on failure
					while (result_idx > 0)
						free(result[--result_idx]);
					free(result);
					return (NULL);
				}
				result_idx++;
				start = i;
				in_space = 0;
			}
		}
		i++;
	}
	
	// Add final segment
	if (start < i)
	{
		result[result_idx] = ft_substr(s, start, i - start);
		if (!result[result_idx])
		{
			// Free already allocated strings on failure
			while (result_idx > 0)
				free(result[--result_idx]);
			free(result);
			return (NULL);
		}
		result_idx++;
	}
	
	result[result_idx] = NULL;
	return (result);
}
