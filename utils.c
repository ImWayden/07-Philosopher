/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 16:19:02 by wayden            #+#    #+#             */
/*   Updated: 2023/10/24 18:14:46 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"


t_bool	is_even(int i)
{
	return (i % 2 == 0);
}

t_bool	is_digit(char c)
{
	return (c >= '0' && c <= '9');
}

t_ms_time	atoi_error(const char *str, t_error err_code)
{
	t_ms_time	result;
	int			i;

	i = 0;
	result = 0;
	while (str[i])
	{
		if (!is_digit(str[i]))
		{
			*sget_error() = err_code;
			break ;
		}
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (result);
}

int	mutexed_print(int id, t_task task)
{
	t_state		*state;
	const char	*str;

	state = sget_state();
	pthread_mutex_lock(&state->mutex_print);
	if (check_state(id))
		return (pthread_mutex_unlock(&state->mutex_print), 0);
	if (task == EATING)
		str = "is eating";
	else if (task == SLEEPING)
		str = "is sleeping";
	else if (task == THINKING)
		str = "is thinking";
	else if (task == FORKING)
		str = "has taken a fork";
	else
		str = "started";
	printf("%lldms %d %s\n", get_local_cur_t(), id + 1, str);
	return (pthread_mutex_unlock(&state->mutex_print), 0);
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
