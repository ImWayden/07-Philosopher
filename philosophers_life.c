/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_life.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/20 20:54:52 by wayden            #+#    #+#             */
/*   Updated: 2023/10/21 04:01:33 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

void mutexed_print(int id, t_task task)
{
    t_state *state;
    const char *str;

    state = sget_state();
    pthread_mutex_lock(&state->mutex_stop);
    if(state->stop)
    {
        pthread_mutex_unlock(&state->mutex_stop);
        return;
    }
    pthread_mutex_unlock(&state->mutex_stop);
    if (task == EATING)
        str = "is eating";
    else if (task == SLEEPING)
        str = "is sleeping";
    else if (task == THINKING)
        str = "is thinking";
    else if (task == DYING)
        str = "died";
    else
        str = "playing subway surfer";
    pthread_mutex_lock(&state->mutex_print);
    printf("%20lld %03d %s\n", get_local_cur_t(), id + 1, str);
    pthread_mutex_unlock(&state->mutex_print);
}

t_bool trylock_fork(int id_l_fork, int id_r_fork)
{
    t_philosophe *philosopher;
    t_bool has_fork;

    has_fork = FALSE;
    philosopher = sget_philosophers();
    pthread_mutex_lock(&philosopher[id_l_fork].mutex_fork);
    pthread_mutex_lock(&philosopher[id_r_fork].mutex_fork);
    if (philosopher[id_l_fork].fork && philosopher[id_r_fork].fork)
    {
        philosopher[id_l_fork].fork = FALSE;
        philosopher[id_l_fork].fork = FALSE;
        has_fork = TRUE;
    }
    pthread_mutex_unlock(&philosopher[id_l_fork].mutex_fork);
    pthread_mutex_unlock(&philosopher[id_r_fork].mutex_fork);
    return (has_fork);
}

t_bool check_if_dead(int id)
{
    t_philosophe *philo;
    t_ms_time time_to_die;

    philo = sget_philosophers();
    time_to_die = sget_args(NULL)->time2die;
    if (get_laps_t(philo[id].task.eat.last_time, get_cur_t()) > time_to_die)
    {
        mutexed_print(id, DYING);
        return (TRUE);
    }
    else
        return (FALSE);
}

int eating(int id)
{
    t_philosophe *philo;
    t_ms_time time2eat;
    t_ms_time laps_time;
    int id_l_fork;
    int id_r_fork;

    time2eat = sget_args(NULL)->time2eat;
    philo = sget_philosophers();
    id_l_fork = id;
    id_r_fork = (id + 1) % sget_args(NULL)->nb_philo;
    while (!trylock_fork(id_l_fork, id_r_fork))
        if (check_if_dead(id))
            return (1);
    philo[id].task.eat.start_time = get_cur_t();
    mutexed_print(id, EATING);
    while (get_laps_t(philo[id].task.eat.start_time, get_cur_t()) < time2eat)
        if (check_if_dead(id))
            return (1);
    philo[id].task.eat.last_time = get_cur_t();
    philo[id].nb_meal++;
    pthread_mutex_lock(&sget_state()->mutex_meal);
    sget_state()->total_meal++;
    pthread_mutex_unlock(&sget_state()->mutex_meal);
    return (0);
}

int sleeping(int id)
{
    t_philosophe *philo;
    t_ms_time time2sleep;
    
    time2sleep = sget_args(NULL)->time2sleep;
    philo = sget_philosophers();
    philo[id].task.sleep.start_time = get_cur_t();
    mutexed_print(id, SLEEPING);
    while (get_laps_t(philo[id].task.sleep.start_time, get_cur_t()) < time2sleep)
        if (check_if_dead(id))
            return (id);
    return(0);
}

void *philosophers_life(void *vo_id)
{
    t_state *state;
    t_philosophe *philosopher;
    t_timeval timeval;
    int *id;
    
    id = (int *)vo_id;
    state = sget_state();
    pthread_mutex_lock(&state->mutex_stop);
    while (!state->stop)
    {
        pthread_mutex_unlock(&state->mutex_stop);
        mutexed_print(*id, THINKING);
        if (eating(*id) && pthread_mutex_lock(&state->mutex_stop))
            break;
        if (sleeping(*id) && pthread_mutex_lock(&state->mutex_stop))
            break;
        pthread_mutex_lock(&state->mutex_stop);
    }
    pthread_mutex_unlock(&state->mutex_stop);
    return(NULL);
}
