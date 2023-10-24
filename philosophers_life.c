/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_life.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/20 20:54:52 by wayden            #+#    #+#             */
/*   Updated: 2023/10/24 18:07:18 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

/*
**	locl_fork
**	lock the fork mutex
**	
*/
static void	lock_fork(int id_l_fork, int id_r_fork, t_philosophe *philosopher)
{

	if (is_even(id_l_fork))
	{
		pthread_mutex_lock(&philosopher[id_l_fork].mutex_fork);
		pthread_mutex_lock(&philosopher[id_r_fork].mutex_fork);
	}
	else
	{
		pthread_mutex_lock(&philosopher[id_r_fork].mutex_fork);
		pthread_mutex_lock(&philosopher[id_l_fork].mutex_fork);
	}
	mutexed_print(id_l_fork, FORKING);
	mutexed_print(id_l_fork, FORKING);
}

/*
**	unlock forks
**	so other threads can use them
**	
*/
static void	unlock_fork(int id_l_fork, int id_r_fork, t_philosophe *philosopher)
{
	if (is_even(id_l_fork))
	{
		pthread_mutex_unlock(&philosopher[id_l_fork].mutex_fork);
		pthread_mutex_unlock(&philosopher[id_r_fork].mutex_fork);
	}
	else
	{
		pthread_mutex_unlock(&philosopher[id_r_fork].mutex_fork);
		pthread_mutex_unlock(&philosopher[id_l_fork].mutex_fork);
	}
}

/*
**	all related eating actions
**	eating
**	
*/
static int	eating(int id, t_philosophe	*philo)
{
	t_ms_time		time2eat;
	t_ms_time		start_eating;

	time2eat = sget_args(NULL)->time2eat;
	start_eating = get_cur_t();
	mutexed_print(id, EATING);
	while (get_laps_t(start_eating, get_cur_t()) < time2eat)
		usleep(0.1);
	if (check_state(id))
		return (unlock_fork(id, philo[id].fork_right_id, philo),1);
	philo[id].last_meal = get_cur_t();
	philo[id].nb_meal++;
	if (philo[id].nb_meal == sget_args(NULL)->nb_eating)
	{
		pthread_mutex_lock(&philo[id].mutex_finished);
		philo[id].has_finished = TRUE;
		pthread_mutex_unlock(&philo[id].mutex_finished);
		return (unlock_fork(id, philo[id].fork_right_id, philo), 1);
	}

	return (0);
}

/*
**	just wait until sleep is over
** 	while checking if the philosopher does not die during it's sleep
**
*/
static int	sleeping(int id)
{
	t_ms_time		time2sleep;
	t_ms_time		start_time;

	time2sleep = sget_args(NULL)->time2sleep;
	start_time = get_cur_t();

	mutexed_print(id, SLEEPING);
	while (get_laps_t(start_time, get_cur_t()) < time2sleep)
		usleep(0.1);
	if (check_state(id))
		return (1);
	return (0);
}

/*
**	life is the main thread loop
**	ending only when one of the philospher is dead
**	
*/
void	*life(void *vo_id)
{
	int				*id;
	t_philosophe	*philo;
	t_argsphilo		*args;

	philo = sget_philo();
	args = sget_args(NULL);
	id = (int *)vo_id;
	if (!is_even(*id))
		usleep(args->time2eat * 999);
	while ((!check_state(*id)))
	{
		lock_fork(*id, philo[*id].fork_right_id, philo);
		if (eating(*id, philo))
			break ;
		unlock_fork(*id, philo[*id].fork_right_id, philo);
		if (sleeping(*id))
			break ;
		if (!check_state(*id))
			mutexed_print(*id, THINKING);
	}
	return (NULL);
}
