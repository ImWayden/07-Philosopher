/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   singletons.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/20 16:25:54 by wayden            #+#    #+#             */
/*   Updated: 2023/10/22 21:04:29 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

t_argsphilo	*sget_args(char **argv)
{
	static t_argsphilo	args;
	static t_bool		initialized = FALSE;

	if (!initialized)
	{
		args.nb_philo = atoi(argv[1]);
		args.time2die = atoi(argv[2]);
		args.time2eat = atoi(argv[3]);
		args.time2sleep = atoi(argv[4]);
		if (argv[5])
			args.nb_eating = atoi(argv[5]);
		else
			args.nb_eating = -1;
		initialized = TRUE;
	}
	return (&args);
}

t_philosophe	*sget_philo(void)
{
	static t_philosophe	*philosophers;
	static t_bool		initialized = FALSE;
	int					nb_philo;
	int					i;

	i = -1;
	nb_philo = sget_args(NULL)->nb_philo;
	if (!initialized)
	{
		philosophers = (t_philosophe *)malloc(sizeof(t_philosophe) * nb_philo);
		if (philosophers == NULL)
			return (NULL);
		memset(philosophers, 0, sizeof(t_philosophe) * nb_philo);
		while (++i < nb_philo)
		{
			if (pthread_mutex_init(&philosophers[i].mutex_fork, NULL) != 0)
				return (free(philosophers), NULL);
			philosophers[i].id = i;
			philosophers[i].fork = TRUE;
		}
		initialized = TRUE;
	}
	return (philosophers);
}

t_state	*sget_state(void)
{
	static t_state	state;
	static t_bool	initialized = FALSE;

	if (!initialized)
	{
		state.total_meal = 0;
		state.stop = FALSE;
		if (pthread_mutex_init(&state.mutex_stop, NULL) != 0)
			return (NULL);
		if (pthread_mutex_init(&state.mutex_print, NULL) != 0)
			return (NULL);
		if (pthread_mutex_init(&state.mutex_meal, NULL) != 0)
			return (NULL);
		if (pthread_mutex_init(&state.mutex_time, NULL) != 0)
			return (NULL);
		initialized = TRUE;
	}
	return (&state);
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
