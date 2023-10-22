/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_life.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/20 20:54:52 by wayden            #+#    #+#             */
/*   Updated: 2023/10/22 14:57:51 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

void mutexed_print(int id, t_task task)
{
    t_state *state;
    const char *str;

    state = sget_state();
    if(check_state())
        return;
    if (task == EATING)
        str = "is eating";
    else if (task == SLEEPING)
        str = "is sleeping";
    else if (task == THINKING)
        str = "is thinking";
    else if (task == DYING)
    {
        str = "died";
        stop();
    }
    else
        str = "playing subway surfer";
    pthread_mutex_lock(&state->mutex_print);
    printf("%lld %03d %s\n", get_local_cur_t(), id + 1, str);
    pthread_mutex_unlock(&state->mutex_print);
}

t_bool check_if_dead(int id)
{
    t_philosophe *philo;
    t_state *state;
    t_ms_time time_to_die;
    
    philo = sget_philosophers();
    state = sget_state();
    time_to_die = sget_args(NULL)->time2die;
    //printf("2 id: %d start_time : %lld last_time : %lld time2die :%lld laps: %lld\n",id ,state->global_time.start_time,philo[id].last_meal, time_to_die, laps);//debug

    if (get_laps_t(philo[id].last_meal, get_cur_t()) > time_to_die)
        return (mutexed_print(id, DYING),TRUE);
    else
        return (FALSE);
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
        philosopher[id_r_fork].fork = FALSE;
        has_fork = TRUE;
    }
    pthread_mutex_unlock(&philosopher[id_l_fork].mutex_fork);
    pthread_mutex_unlock(&philosopher[id_r_fork].mutex_fork);
    return (has_fork);
}

int eating(int id)
{
    t_philosophe *philo;
    t_ms_time time2eat;
    t_ms_time start_eating;

    time2eat = sget_args(NULL)->time2eat;
    philo = sget_philosophers();
    while (!trylock_fork(id, (id + 1) % sget_args(NULL)->nb_philo))
        if (check_if_dead(id))
            return (1);
    start_eating = get_cur_t();
    if(!check_state())
        mutexed_print(id, EATING);
    while (get_laps_t(start_eating, get_cur_t()) < time2eat)
        if (check_if_dead(id))
            return (1);
    philo[id].last_meal = get_cur_t();
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
    t_ms_time start_time;
    
    time2sleep = sget_args(NULL)->time2sleep;
    philo = sget_philosophers();
    start_time = get_cur_t();
    if(!check_state())
        mutexed_print(id, SLEEPING);
    while (get_laps_t(start_time, get_cur_t()) < time2sleep)
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
    
    while (!check_state())
    {
        mutexed_print(*id, THINKING);
        if (eating(*id))
            break;
        if (sleeping(*id))
            break;
    }
    return(NULL);
}
