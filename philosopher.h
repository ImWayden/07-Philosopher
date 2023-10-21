/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/19 21:30:04 by wayden            #+#    #+#             */
/*   Updated: 2023/10/21 18:35:56 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHIlOSOPHER_H
#define PHILOSOPHER_H

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <sys/time.h>
#include <sys/types.h>

#define MUTEX pthread_mutex_t
#define THREAD pthread_t

typedef enum e_value
{
    FALSE = 0,
    TRUE = 1
} t_bool;

typedef enum e_task
{
    EATING = 1,
    SLEEPING = 2,
    THINKING = 3,
    DYING = 4,
    DEBUG = 5
}   t_task;

enum e_error
{
    ERR_PTHREAD_CREATE = 1,
};

typedef struct timeval t_timeval;

typedef long long int t_ms_time;

typedef struct s_time
{
    t_ms_time start_time;
    t_ms_time last_time;
    t_ms_time elapsed_time;
}   t_timers;

typedef struct s_tasks
{
    t_timers eat;
    t_timers sleep;
    t_timers think;
} t_tasks;

typedef struct s_philosophe
{
    pthread_t thread;
    pthread_mutex_t mutex_fork;
    t_tasks task;
    t_bool fork;
    int id;
    int nb_meal;
} t_philosophe;

typedef struct s_state
{
    pthread_mutex_t mutex_stop;
    pthread_mutex_t mutex_print;
    pthread_mutex_t mutex_meal;
    t_bool stop;
    t_timers global_time;
    int total_meal;
} t_state;

typedef struct s_argsphilo
{
    int nb_philo;
    int nb_eating;
    t_ms_time time2die;
    t_ms_time time2eat;
    t_ms_time time2sleep;
} t_argsphilo;

typedef struct s_treaddata
{
    pthread_mutex_t mutex_args;
    pthread_mutex_t mutex_philo;
    pthread_mutex_t mutex_state;
    t_argsphilo *args;
    t_philosophe *philo;
    t_state *state;
    
}   t_threaddata;

// typedef struct s_states
// {
//     pthread_t *id_philosopher;
//     pthread_mutex_t *mutex_fork;
//     pthread_mutex_t mutex_stop;
//     bool *forks;
//     bool stop;
// } t_states;


t_argsphilo *sget_args(char **argv);
t_philosophe *sget_philosophers(void);
t_state *sget_state(void);

t_ms_time get_cur_t();
t_ms_time get_laps_t(t_ms_time start_time, t_ms_time current_time);
t_ms_time get_local_cur_t();

void *philosophers_life(void *vo_id);

#endif