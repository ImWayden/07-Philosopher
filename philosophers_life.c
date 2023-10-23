/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_life.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/20 20:54:52 by wayden            #+#    #+#             */
/*   Updated: 2023/10/24 00:58:03 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

/*
**	try to lock the forks
**	return TRUE or FALSE
**	is really useless way to use variable to store the state of forks
**	when i could simply lock the mutex_forks until philo finished eating
*/
t_bool	can_lock_fork(int id_l_fork, int id_r_fork, t_philosophe *philosopher)
{
	t_bool	has_fork;

	has_fork = FALSE;
	pthread_mutex_lock(&philosopher[id_l_fork].mutex_fork);
	pthread_mutex_lock(&philosopher[id_r_fork].mutex_fork);
	if (philosopher[id_r_fork].fork && philosopher[id_l_fork].fork)
	{
		philosopher[id_l_fork].fork = FALSE;
		mutexed_print(id_l_fork, FORKING);
		philosopher[id_r_fork].fork = FALSE;
		mutexed_print(id_l_fork, FORKING);
		has_fork = TRUE;
	}
	pthread_mutex_unlock(&philosopher[id_l_fork].mutex_fork);
	pthread_mutex_unlock(&philosopher[id_r_fork].mutex_fork);
	return (has_fork);
}

/*
**	unlock forks
**	so other threads can use them
**	
*/
void	unlock_fork(int id_l_fork, int id_r_fork, t_philosophe *philosopher)
{
	pthread_mutex_lock(&philosopher[id_l_fork].mutex_fork);
	pthread_mutex_lock(&philosopher[id_r_fork].mutex_fork);
	philosopher[id_l_fork].fork = TRUE;
	philosopher[id_r_fork].fork = TRUE;
	pthread_mutex_unlock(&philosopher[id_l_fork].mutex_fork);
	pthread_mutex_unlock(&philosopher[id_r_fork].mutex_fork);
}

/*
**	all related eating actions
**	try to take forks if yes start eating else continue trying
**	
*/
int	eating(int id, t_philosophe	*philo)
{
	t_ms_time		time2eat;
	t_ms_time		start_eating;

	time2eat = sget_args(NULL)->time2eat;
	start_eating = get_cur_t();
	mutexed_print(id, EATING);
	while (get_laps_t(start_eating, get_cur_t()) < time2eat)
		usleep(1);
	philo[id].last_meal = get_cur_t();
	philo[id].nb_meal++;
	pthread_mutex_lock(&sget_state()->mutex_meal);
	sget_state()->total_meal++;
	pthread_mutex_unlock(&sget_state()->mutex_meal);
	return (0);
}

/*
**	just wait until sleep is over
** 	while checking if the philosopher does not die during it's sleep
**
*/
int	sleeping(int id)
{
	t_philosophe	*philo;
	t_ms_time		time2sleep;
	t_ms_time		start_time;

	time2sleep = sget_args(NULL)->time2sleep;
	philo = sget_philo();
	start_time = get_cur_t();

	mutexed_print(id, SLEEPING);
	while (get_laps_t(start_time, get_cur_t()) < time2sleep)
		usleep(1);
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
	while (!check_state())
	{
			
		while (!can_lock_fork(*id, philo[*id].fork_right_id, philo))
			if(check_state())
				break ;
		if (check_state() || eating(*id,philo))
			break ;
		unlock_fork(*id, philo[*id].fork_right_id, philo);
		if (check_state() || sleeping(*id))
			break ;
		if (!check_state())
			mutexed_print(*id, THINKING);
	}
	return (NULL);
}
