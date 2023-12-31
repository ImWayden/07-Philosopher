/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/21 00:44:42 by wayden            #+#    #+#             */
/*   Updated: 2023/10/25 17:59:19 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosopher.h"

/*
**  get_current_time (name shorten because of the norm)
**  returns current time in ms
*/
t_ms_time	get_cur_t(void)
{
	t_timeval	cur_time;

	gettimeofday(&cur_time, NULL);
	return ((t_ms_time)(cur_time.tv_sec * 1000 + cur_time.tv_usec / 1000));
}

/*
**  get_elapsed_time (name shorten because of the norm)
**  returns elapsed time beetween start_time and current_time;
*/
t_ms_time	get_laps_t(t_ms_time start_time, t_ms_time current_time)
{
	return ((t_ms_time)(current_time - start_time));
}

/*
**  get_local_current_time (name shorten because of the norm)
**  returns current time in ms
**  time is global and not local to the app
*/
t_ms_time	get_local_cur_t(void)
{
	return ((t_ms_time)(get_cur_t() - sget_state()->global_start));
}
