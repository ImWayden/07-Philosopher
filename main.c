/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/19 20:32:46 by wayden            #+#    #+#             */
/*   Updated: 2023/10/22 22:17:45 by wayden           ###   ########.fr       */
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

void	error_manager(int error)
{
	return ;
}

int	main(int argc, char **argv)
{
	t_argsphilo		*args;
	t_philosophe	*philo;
	t_state			*state;
	void			*returnvalue;
	int				i;

	if (argc < 4)
		return (0);
	i = -1;
	args = sget_args(argv);
	state = sget_state();
	philo = sget_philo();
	state->global_start = get_cur_t();
	while (++i < args->nb_philo)
	{
		pthread_create(&philo[i].thread, NULL, life, (void *)&philo[i].id);
		usleep(100);
	}
	i = -1;
	while (++i < args->nb_philo)
	{
		pthread_join(philo[i].thread, &returnvalue);
	}
	free(philo);
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
