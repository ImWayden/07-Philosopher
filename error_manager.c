/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_manager.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 16:25:40 by wayden            #+#    #+#             */
/*   Updated: 2023/10/25 14:30:06 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"


static void	error_print_arg(t_error error)
{
	if (error == ERR_NB_ARG)
		printf ("Usage: ./philo <nb_philo> <time2die> <time2eat> \
<time2sleep> [nb_eating]\n");
	else
		printf("error in arg %d, non digits caracter aren't allowed\n", error);
}

static void	error_print_thread(t_error error)
{
	if (error == ERR_PTHREAD_CREATE)
		printf("error : pthread_create failed\n");
	else if (error == ERR_PTHREAD_DETACH)
		printf("error : pthread_detach failed\n");
	else
		printf("error : pthread_join failed\n");
}

static void error_print_mutex(void)
{
	printf("error : mutex init failed\n");
}

t_bool	error_manager(void)
{
	t_error	*error;

	error = sget_error();
	if (!*error)
		return (FALSE);
	else if (*error <= ERR_NB_ARG)
		return (error_print_arg(*error), TRUE);
	else if (*error >= ERR_PTHREAD_CREATE && *error <= ERR_PTHREAD_JOIN)
		return (error_print_thread(*error), TRUE);
	else if (*error == ERR_MUTEX_INIT_00)
		return (error_print_mutex(), TRUE);
	else
		return (TRUE);
	return (FALSE);
}

