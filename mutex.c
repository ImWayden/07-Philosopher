/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 14:07:04 by wayden            #+#    #+#             */
/*   Updated: 2023/10/24 18:13:03 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"


t_philosophe	*mutex_init(void)
{
	t_philosophe	*philo;
	int				nb_philo;
	int				i;

	i = -1;
	philo = sget_philo();
	nb_philo = sget_args(NULL)->nb_philo;
	while (++i < nb_philo)
	{
		if (pthread_mutex_init(&philo[i].mutex_fork, NULL) != 0)
			return (*sget_error() = ERR_MUTEX_INIT, free(philo), NULL);
		if (pthread_mutex_init(&philo[i].mutex_finished, NULL) != 0)
			return (*sget_error() = ERR_MUTEX_INIT, free(philo), NULL);
	}
	return(philo);
}

t_bool	check_state(int id)
{
	t_bool			should_stop;
	t_philosophe	*philo;
	t_state			*state;

	state = sget_state();
	philo = sget_philo();
	if (philo[id].nb_meal == sget_args(NULL)->nb_eating)
		return (TRUE);
	pthread_mutex_lock(&state->mutex_stop);
	if (state->stop)
		should_stop = TRUE;
	else
		should_stop = FALSE;
	pthread_mutex_unlock(&state->mutex_stop);
	return (should_stop);
}

void	stop(void)
{
	t_state	*state;

	state = sget_state();
	pthread_mutex_lock(&state->mutex_stop);
	state->stop = 1;
	pthread_mutex_unlock(&state->mutex_stop);
}
