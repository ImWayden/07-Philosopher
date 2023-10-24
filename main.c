/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/19 20:32:46 by wayden            #+#    #+#             */
/*   Updated: 2023/10/24 18:28:38 by wayden           ###   ########.fr       */
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
#include "philosopher.h"

void	kill_all()
{
	t_argsphilo		*args;
	t_philosophe	*philo;
	t_state			*state;
	int 			i;

	i = -1;
	args = sget_args(NULL);
	state = sget_state();
	philo = sget_philo();
	while (++i < args->nb_philo)
	{
		pthread_detach(philo[i].thread);
		pthread_mutex_destroy(&philo[i].mutex_fork);
		pthread_mutex_destroy(&philo[i].mutex_finished);
	}
	pthread_mutex_destroy(&state->mutex_meal);
	pthread_mutex_destroy(&state->mutex_print);
	pthread_mutex_destroy(&state->mutex_stop);
}




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
		pthread_mutex_lock(&philo[i].mutex_finished);
		if (philo[i].has_finished)
			finished++;
		pthread_mutex_unlock(&philo[i].mutex_finished);
		if (finished == args->nb_philo)
			return (usleep(100), FALSE);
		else if (last_meal > args->time2die)
		{
			current_time = get_local_cur_t();
			pthread_mutex_lock(&state->mutex_print);
			stop();
			printf("%lldms %d died\n", current_time, i + 1);
			pthread_mutex_unlock(&state->mutex_print);
			return (FALSE);
		}
	}
	return (TRUE);
}

t_bool	sit_at_the_table(t_argsphilo *args, t_philosophe *phi, t_state *state)
{
	int			i;

	i = -1;
	state->global_start = get_cur_t();
	while (++i < args->nb_philo)
	{
		if (pthread_create(&phi[i].thread, NULL, life, (void *)&phi[i].id))
			return (*sget_error() = ERR_PTHREAD_CREATE, FALSE);
		usleep(10);
	}
	return (TRUE);
}

t_bool	init(char **argv)
{
	t_argsphilo		*args;
	t_philosophe	*philo;
	t_state			*state;

	args = sget_args(argv);
	state = sget_state();
	philo = sget_philo();
	if (error_manager())
		return (FALSE);
	if (!sit_at_the_table(args, philo, state))
		return (free(philo), !error_manager());
	return (TRUE);
}

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
	printf("%lld simulation ended\n", get_local_cur_t());
	return (free(philo), 0);
}

// int main(int argc, char **argv)
// {
//     t_argsphilo *args;
//     t_philosophe *philosophers;
//     int i;

//     if (argc < 4)
//         return (0);
//     i = -1;
//     args = sget_args(argv);
//     philosophers = sget_philo();

//     if (!philosophers)
//     {
//         perror("Erreur lors de l'initialisation des philosophes");
//         return 1;
//     }

//     // Testez l'initialisation en affichant les états des philosophes
//     int num_philosophers = sget_args(NULL)->nb_philo+1;
//     for (int i = 0; i < num_philosophers; i++)
//     {
//         printf("Philosophe %d - Mutex initialisé: %s\n", i, pthread_mutex_trylock(&philosophers[i].mutex_fork) == 0 ? "Oui" : "Non");
//         printf("Philosophe %d - Fourchette: %s\n", i, philosophers[i].fork ? "Oui" : "Non");
//     }

//     // Nettoyez les ressources (mutex)
//     for (int i = 0; i < num_philosophers; i++)
//     {
//         pthread_mutex_destroy(&philosophers[i].mutex_fork);
//     }

//     free(philosophers); // Libérez la mémoire allouée pour les philosophes

//     printf("Test terminé.\n");

//     return 0;
// }

// int main(int argc, char *argv[]) {
//     if (argc < 5) {
//         printf("Usage: %s <nb_philo> <time2die> <time2eat> <time2sleep> [nb_eating]\n", argv[0]);
//         return 1;
//     }
//     sget_Args(argv);
//     // Obtenir le singleton des arguments des philosophes
//     t_argsphilo *philo_args;
//     philo_args = sget_Args(argv);
//     // Afficher les arguments
//     printf("Nombre de philosophes: %d\n", philo_args->nb_philo);
//     printf("Temps pour mourir: %u us\n", philo_args->time2die);
//     printf("Temps pour manger: %u us\n", philo_args->time2eat);
//     printf("Temps pour dormir: %u us\n", philo_args->time2sleep);
//     printf("Nombre de repas (si spécifié): %d\n", philo_args->nb_eating);

//     return 0;
// }
