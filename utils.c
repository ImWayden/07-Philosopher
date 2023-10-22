/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 16:19:02 by wayden            #+#    #+#             */
/*   Updated: 2023/10/22 21:26:43 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

void	mutexed_print(int id, t_task task)
{
	t_state		*state;
	const char	*str;

	state = sget_state();
	if (check_state())
		return ;
	if (task == EATING)
		str = "is eating";
	else if (task == SLEEPING)
		str = "is sleeping";
	else if (task == THINKING)
		str = "is thinking";
	else if (task == FORKING)
		str = "has taken a fork";
	else if (task == DYING)
	{
		str = "died";
		stop();
	}
	else
		str = "playing subway surfer";
	pthread_mutex_lock(&state->mutex_print);
	printf("%lld %d %s\n", get_local_cur_t(), id + 1, str);
	pthread_mutex_unlock(&state->mutex_print);
}

/*
**	Check if a philosopher has eaten 
**	if not print death message
**	
*/
t_bool	is_dead(int id)
{
	t_philosophe	*philo;
	t_ms_time		time_to_die;

	philo = sget_philo();
	time_to_die = sget_args(NULL)->time2die;
	if (get_laps_t(philo[id].last_meal, get_cur_t()) > time_to_die)
		return (mutexed_print(id, DYING), TRUE);
	else
		return (FALSE);
}
