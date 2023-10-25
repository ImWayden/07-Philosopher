/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   singletons.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/20 16:25:54 by wayden            #+#    #+#             */
/*   Updated: 2023/10/25 14:49:36 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"


t_argsphilo	*sget_args(char **argv)
{
	static t_argsphilo	args;
	static t_bool		initialized = FALSE;

	if (!initialized)
	{
		args.nb_philo = atoi_error(argv[1], ERR_PARSING_ARG1);
		args.time2die = atoi_error(argv[2], ERR_PARSING_ARG2);
		args.time2eat = atoi_error(argv[3], ERR_PARSING_ARG3);
		args.time2sleep = atoi_error(argv[4], ERR_PARSING_ARG4);
		if (argv[5] != NULL)
			args.nb_eating = atoi_error(argv[5], ERR_PARSING_ARG5);
		else
			args.nb_eating = -1;
		initialized = TRUE;
	}
	return (&args);
}

t_philosophe	*sget_philo(void)
{
	static t_philosophe	*philo;
	static t_bool		initialized = FALSE;
	int					nb_philo;
	int					i;

	i = -1;
	nb_philo = sget_args(NULL)->nb_philo;
	if (!initialized)
	{
		philo = (t_philosophe *)malloc(sizeof(t_philosophe) * nb_philo);
		if (philo == NULL)
			return (initialized = TRUE, *sget_error() = ERR_MALLOC, NULL);
		memset(philo, 0, sizeof(t_philosophe) * nb_philo);
		while (++i < nb_philo)
		{
			philo[i].id = i;
			philo[i].last_meal = get_cur_t();
			philo[i].fork_right_id = (i + 1) % sget_args(NULL)->nb_philo;
		}
		initialized = TRUE;
		return (mutex_init());
	}
	return (philo);
}

t_state	*sget_state(void)
{
	static t_state	state;
	static t_bool	initialized = FALSE;

	if (!initialized)
	{
		state.stop = FALSE;
		memset(&state.state_tab, 0, sizeof(pthread_mutex_t *) * 2);
		if (pthread_mutex_init(&state.mutex_stop, NULL) != 0)
			return (*sget_error() = ERR_MUTEX_INIT_00, NULL);
		state.state_tab[0] = &state.mutex_stop;
		if (pthread_mutex_init(&state.mutex_print, NULL) != 0)
			return (*sget_error() = ERR_MUTEX_INIT_00, NULL);
		state.state_tab[0] = &state.mutex_print;
		state.global_start = get_cur_t();
		initialized = TRUE;
	}
	return (&state);
}

t_error	*sget_error(void)
{
	static t_error	error = NO_ERROR;

	return (&error);
}
// t_threaddata *sget_threaddata

// t_states *sget_ids(void)
// {
//     static t_states philo;
//     static bool initialized = FALSE;
//     int nb_philo;

//     nb_philo = sget_args(NULL)->nb_philo + 1;
//     if(!initialized)
//     {
//         philo.id_philosopher = (THREAD *)malloc(sizeof(THREAD) * (nb_philo));
//         if(philo.id_philosopher == NULL)
//             return(NULL);
//         memset(philo.id_philosopher,0,sizeof(THREAD) * (nb_philo));
//         philo.mutex_fork = (MUTEX *)malloc(sizeof(MUTEX) * (nb_philo));
//         if(philo.mutex_fork == NULL)
//             return(free(philo.id_philosopher), NULL);
//         memset(philo.mutex_fork,0,sizeof(MUTEX) * (nb_philo));
//         philo.forks = (bool *)malloc(sizeof(bool) * (nb_philo));
//         if(philo.forks == NULL)
//             return(free(philo.mutex_fork),free(philo.id_philosopher), NULL);
//         memset(philo.forks,FALSE,sizeof(bool) * (nb_philo));
//         philo.stop = FALSE;
//         initialized = TRUE;
//     }
//     return (&philo);
// }
