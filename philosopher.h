/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/19 21:30:04 by wayden            #+#    #+#             */
/*   Updated: 2023/10/24 18:03:02 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHER_H
# define PHILOSOPHER_H

# include <stdio.h>
# include <pthread.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <malloc.h>
# include <sys/time.h>
# include <sys/types.h>
/*
** Boolean type for more readability
*/
typedef enum e_value
{
	FALSE = 0,
	TRUE = 1
}	t_bool;

/*
** enum for task for readability
*/
typedef enum e_task
{
	EATING = 1,
	SLEEPING = 2,
	THINKING = 3,
	FORKING = 4,
	DYING = 5,
	DEBUG = 6
}	t_task;
/*
** enum for errors for readability
*/
typedef enum e_error
{
	NO_ERROR = 0,
	ERR_PARSING_ARG1 = 1,
	ERR_PARSING_ARG2 = 2,
	ERR_PARSING_ARG3 = 3,
	ERR_PARSING_ARG4 = 4,
	ERR_PARSING_ARG5 = 5,
	ERR_NB_ARG = 6,
	ERR_PTHREAD_CREATE = 7,
	ERR_PTHREAD_JOIN = 8,
	ERR_PTHREAD_DETACH = 9,
	ERR_MUTEX_INIT = 10,
	ERR_MALLOC = 11
}	t_error;
/*
**	to avoid having to write "struct"
*/
typedef struct timeval	t_timeval;
/*
**	because long long int is too long
**	and reduce readability
** 	plus it's easier to change the type if this one isn't adapted
*/
typedef long long int	t_ms_time;

/*
** this struct is kinda  an philosopher object
** should be initialized after state and args
*/
typedef struct s_philosophe
{
	pthread_t		thread;
	pthread_mutex_t	mutex_fork;
	pthread_mutex_t	mutex_finished;
	t_ms_time		last_meal;
	t_bool			has_finished;
	int				id;
	int				fork_right_id;
	int				nb_meal;
}	t_philosophe;
/*
** state just a struct made to store the state related variables
** should be initialized before philo struct
*/
typedef struct s_state
{
	pthread_mutex_t	mutex_stop;
	pthread_mutex_t	mutex_print;
	pthread_mutex_t	mutex_meal;

	t_ms_time		global_start;
	t_bool			stop;
	int				total_meal;
}	t_state;
/*
** argsphilo just a struct made to store the args
** should be initialized before philo struct
*/
typedef struct s_argsphilo
{
	int			nb_philo;
	int			nb_eating;
	t_ms_time	time2die;
	t_ms_time	time2eat;
	t_ms_time	time2sleep;
}	t_argsphilo;
/*
** singletons part
*/
t_argsphilo		*sget_args(char **argv);
t_philosophe	*sget_philo(void);
t_state			*sget_state(void);
t_error			*sget_error(void);
/*
** time management part
*/
t_ms_time		get_cur_t(void);
t_ms_time		get_laps_t(t_ms_time start_time, t_ms_time current_time);
t_ms_time		get_local_cur_t(void);

void			*life(void *vo_id);
t_bool			error_manager(void);
t_bool			is_even(int i);
t_ms_time		atoi_error(const char *str, t_error err_code);

t_philosophe	*mutex_init(void);
t_bool			check_state(int id);
void			stop(void);
t_bool			is_dead(int id);
int				mutexed_print(int id, t_task task);
#endif