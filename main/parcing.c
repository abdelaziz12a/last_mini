/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parcing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaboudra <aaboudra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 21:40:51 by aaboudra          #+#    #+#             */
/*   Updated: 2025/08/07 18:52:31 by aaboudra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static	int	too_many_heredocs(char *line)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (line[i])
	{
		if (line[i] == '<' && line[i + 1] == '<')
		{
			count++;
			if (count > MAX_HEREDOCS)
			{
				return (1);
			}
			i++;
		}
		i++;
	}
	return (0);
}

static	int	syntax_quote_or_pipe(char *line, t_data *data)
{
	if (unclosed_quote(line))
	{
		printf("syntax error (unclosed quote)\n");
		free(line);
		data->last_exit_status = EXIT_SYNTAX_ERROR;
		return (0);
	}
	if (error_pipe(line))
	{
		printf("syntax error near unexpected token `||'\n");
		free(line);
		data->last_exit_status = EXIT_SYNTAX_ERROR;
		return (0);
	}
	if (too_many_heredocs(line))
	{
		printf("bash: maximum here-document count exceeded\n");
		free(line);
		data->last_exit_status = EXIT_SYNTAX_ERROR;
		return (0);
	}
	return (1);
}

void	parcer_and_exec(t_comand *token, t_data *data)
{
	parse_command(token, data);
	expand_all_commands(data);
	start_execution(data);
}

static void	handle_sigint(t_data *data)
{
	if (g_sigint_received == 1)
	{
		data->last_exit_status = 1;
		g_sigint_received = 100;
	}
}

static void	handle_eof(t_data *data)
{
	printf("exit\n");
	clear_history();
	gc_free_all(data);
	exit(data->last_exit_status);
}

void	parcing(t_data *data)
{
	char		*line;
	t_comand	*token;

	while (1)
	{
		signal_init();
		g_sigint_received = 100;
		line = readline("minishell-$ ");
		handle_sigint(data);
		if (!line)
			handle_eof(data);
		add_history(line);
		if (!syntax_quote_or_pipe(line, data))
			continue ;
		token = tokens(line, data);
		if (!chech_syntax(token, line, data))
			continue ;
		parcer_and_exec(token, data);
		handle_sigint(data);
		free(line);
	}
}
