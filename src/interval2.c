/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interval2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 16:45:19 by lbrusa            #+#    #+#             */
/*   Updated: 2024/10/08 16:18:33 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "interval.h"

/*
 * Returns an empty interval
 */
t_interval	empty_interval(void)
{
	return ((t_interval){0, 0});
}

/*
 * Returns an interval that contains all values
 */
t_interval	universe_interval(void)
{
	return ((t_interval){-1e30, 1e30});
}

double	clamp(t_interval t, double x)
{
	if (x < t.min)
		return (t.min);
	if (x > t.max)
		return (t.max);
	return (x);
}
