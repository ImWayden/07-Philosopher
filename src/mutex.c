/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 14:07:04 by wayden            #+#    #+#             */
/*   Updated: 2023/10/25 17:59:06 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosopher.h"

/*
**	mutex cmp
**	protected comparaison on mutexed value
**
*/
t_bool	mutex_cmp(int *pvar, pthread_mutex_t *mutex, int value)
{
	pthread_mutex_lock(mutex);
	if (*pvar == value)
		return (pthread_mutex_unlock(mutex), TRUE);
	else
		return (pthread_mutex_unlock(mutex), FALSE);
}

/*
**	mutex set
**	protected set on mutexed value
**
*/
void	set_mutex(int *pvar, pthread_mutex_t *mutex, int value)
{
	pthread_mutex_lock(mutex);
	*pvar = value;
	pthread_mutex_unlock(mutex);
}

/*
**	mutex init
**	init the mutexes in the t_philosophe struct
**
*/
t_philosophe	*mutex_init(void)
{
	t_philosophe	*philo;
	int				i;

	i = -1;
	philo = sget_philo();
	while (++i < sget_args(NULL)->nb_philo)
	{
		if (pthread_mutex_init(&philo[i].mutex_fork, NULL) != 0)
			return (*sget_error() = ERR_MUTEX_INIT_00, NULL);
		philo[i].tab[0] = &philo[i].mutex_fork;
		if (pthread_mutex_init(&philo[i].mutex_fin, NULL) != 0)
			return (*sget_error() = ERR_MUTEX_INIT_00, NULL);
		philo[i].tab[1] = &philo[i].mutex_fin;
		if (pthread_mutex_init(&philo[i].mutex_meal, NULL) != 0)
			return (*sget_error() = ERR_MUTEX_INIT_00, NULL);
		philo[i].tab[2] = &philo[i].mutex_meal;
		if (pthread_mutex_init(&philo[i].mutex_last_meal, NULL) != 0)
			return (*sget_error() = ERR_MUTEX_INIT_00, NULL);
		philo[i].tab[3] = &philo[i].mutex_last_meal;
	}
	return (philo);
}

/*
**	Check if a the philosophe should continue
**	or not
**
*/
t_bool	check_state(int id)
{
	t_bool			should_stop;
	t_philosophe	*philo;
	t_state			*state;

	state = sget_state();
	philo = sget_philo();
	if (mutex_cmp((int *)&philo[id].nb_meal,
			&philo[id].mutex_meal, sget_args(NULL)->nb_eating))
		return (TRUE);
	if (mutex_cmp((int *)&state->stop, &state->mutex_stop, TRUE))
		should_stop = TRUE;
	else
		should_stop = FALSE;
	return (should_stop);
}

/*
**	change the control variable to TRUE
**
**
*/
void	stop(void)
{
	t_state	*state;

	state = sget_state();
	set_mutex((int *)&state->stop, &state->mutex_stop, TRUE);
}
