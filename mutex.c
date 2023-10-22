/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 14:07:04 by wayden            #+#    #+#             */
/*   Updated: 2023/10/22 21:06:07 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

t_bool	check_state(void)
{
	t_bool	should_stop;
	t_state	*state;

	state = sget_state();
	pthread_mutex_lock(&state->mutex_stop);
	if (state->stop)
		should_stop = TRUE;
	else
		should_stop = FALSE;
	pthread_mutex_unlock(&state->mutex_stop);
	return (should_stop);
}

void	stop(void)
{
	t_state	*state;

	state = sget_state();
	pthread_mutex_lock(&state->mutex_stop);
	state->stop = 1;
	pthread_mutex_unlock(&state->mutex_stop);
}
