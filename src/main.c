/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/19 20:32:46 by wayden            #+#    #+#             */
/*   Updated: 2023/10/25 18:19:19 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
**  Program Name :
**  philo
**
**  Args:
**  Number_of_Philosophers Time_to_Die time_to_eat Time_To_Sleep
**  Number_of_time_each_philosopher_must_eat
**
**  Allowed Functions:
**  memset, printf, malloc, free, write, usleep, gettimeofday,
**  pthread_create,pthread_detach, pthread_join,
**  pthread_mutex_init, pthread_mutex_destroy, pthread_mutex_lock
**  pthread_mutex_unlock
**
*/
#include "../include/philosopher.h"

/*
**	philo_manager
**	check if a philo is dead or if every philo has eaten
**	if yes stop the simulation
*/
t_bool	philo_manager(t_philosophe *philo, t_state *state, t_argsphilo *args)
{
	int				i;
	int				finished;
	t_ms_time		current_time;
	t_ms_time		last_meal;

	i = -1;
	finished = 0;
	while (++i < args->nb_philo)
	{
		last_meal = get_laps_t(philo[i].last_meal, get_cur_t());
		if (mutex_cmp((int *)&philo[i].has_finished, &philo[i].mutex_fin, TRUE))
			finished++;
		if (finished == args->nb_philo)
			return (usleep(100), FALSE);
		else if (last_meal > args->time2die)
		{
			pthread_mutex_lock(&state->mutex_print);
			current_time = get_local_cur_t();
			stop();
			printf("%lldms %d died\n", current_time, i + 1);
			pthread_mutex_unlock(&state->mutex_print);
			return (FALSE);
		}
	}
	return (TRUE);
}

/*
**	let the threads kill themself 
**	destroy mutex
**
*/
void	kill_all(void)
{
	t_argsphilo		*args;
	t_philosophe	*philo;
	t_state			*state;
	int				i;
	int				j;

	i = -1;
	args = sget_args(NULL);
	state = sget_state();
	philo = sget_philo();
	while (++i < args->nb_philo)
	{
		if (philo[i].thread)
			pthread_join(philo[i].thread, NULL);
		j = -1;
		while (++j < 4)
			if (philo[i].tab[j])
				pthread_mutex_destroy(philo[i].tab[j]);
	}
	j = -1;
	while (++j < 2)
		if (state->state_tab && state->state_tab[j])
			pthread_mutex_destroy(state->state_tab[j]);
	if (philo != NULL)
		free(philo);
}

/*
**	sit_at_the_table(t_argsphilo *args, t_philosophe *phi, t_state *state);
**	generate the threads
**
*/
t_bool	sit_at_the_table(t_argsphilo *args, t_philosophe *phi, t_state *state)
{
	int			i;

	i = -1;
	state->global_start = get_cur_t();
	while (++i < args->nb_philo)
	{
		if (sget_args(NULL)->nb_eating == 0)
			set_mutex((int *)&phi[i].has_finished, &phi[i].mutex_fin, TRUE);
		if (pthread_create(&phi[i].thread, NULL, life, (void *)&phi[i].id))
			return (*sget_error() = ERR_PTHREAD_CREATE, FALSE);
		usleep(10);
	}
	return (TRUE);
}

/*
**	init(char **argv)
**	initialize the singletons the order is important
**	free(philo) if there is an error and print the message
*/
t_bool	init(char **argv)
{
	t_argsphilo		*args;
	t_philosophe	*philo;
	t_state			*state;

	args = sget_args(argv);
	state = sget_state();
	philo = sget_philo();
	if (error_manager())
		return (kill_all(), FALSE);
	if (!sit_at_the_table(args, philo, state))
		return (kill_all(), !error_manager());
	return (TRUE);
}

/*
**	int	main(int argc, char **argv)
**	init then 
**
*/
int	main(int argc, char **argv)
{
	t_argsphilo		*args;
	t_philosophe	*philo;
	t_state			*state;

	if (argc < 5)
		return (*sget_error() = ERR_NB_ARG, error_manager());
	if (!init(argv))
		return (1);
	args = sget_args(NULL);
	state = sget_state();
	philo = sget_philo();
	while (philo_manager(philo, state, args))
		usleep(0.1);
	kill_all();
}
