/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaboudra <aaboudra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 21:52:12 by aaboudra          #+#    #+#             */
/*   Updated: 2025/07/19 13:34:08 by aaboudra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	main(int ac, char **av, char **env)
{
	t_data	*data;

	(void)ac;
	(void)av;
	signal(SIGINT, _sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	data = malloc(sizeof(t_data));
	if (!data)
		exit(EXIT_FAILURE);
	data->last_exit_status = 0;
	data->g_gc_list = NULL;
	data->tenv = gc_malloc(sizeof(t_env), data);
	data->tenv = init_env(env, data);
	parcing(data);
	gc_free_all(data);
	clear_history();
}
