/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/21 00:44:42 by wayden            #+#    #+#             */
/*   Updated: 2023/10/22 14:21:35 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

/*
**  get_current_time (name shorten because of the norm)
**  returns current time in ms
*/
t_ms_time get_cur_t()
{
    t_timeval cur_time;
    
    gettimeofday(&cur_time, NULL);
    usleep(1);
    // printf("s %lld us %lld total %lld\n",(t_ms_time)cur_time.tv_sec, (t_ms_time)cur_time.tv_usec / 1000, test); //debug
    return ((t_ms_time)cur_time.tv_sec * 1000 + (t_ms_time)cur_time.tv_usec / 1000);
}

/*
**  get_elapsed_time (name shorten because of the norm)
**  returns elapsed time beetween start_time and current_time;
*/
t_ms_time get_laps_t(t_ms_time start_time, t_ms_time current_time)
{
    t_ms_time res;
    res = current_time - start_time;
    //printf("LAPS: current_time %lld start time %lld res %lld\n",current_time, start_time, (t_ms_time)(current_time - start_time));//debug
    return(res);
}

/*
**  get_local_current_time (name shorten because of the norm)
**  returns current time in ms
**  time is global and not local to the app
*/
t_ms_time get_local_cur_t()
{
    t_state *state;
    t_ms_time current_t;
    state = sget_state();
    //printf("%lld\n",get_cur_t());
    current_t = get_cur_t();
    // printf("current %lld, start_time %lld, res %lld\n",current_t,state->global_time.start_time,(t_ms_time)(get_cur_t() - state->global_time.start_time));//debug
    return((t_ms_time)(current_t - state->global_time.start_time));
}