/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interval.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 09:59:43 by lbrusa            #+#    #+#             */
/*   Updated: 2024/10/07 16:46:09 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "interval.h"

/*
 * @brief Initializer for an interval
 */
t_interval	interval(double min, double max)
{
	t_interval	i;

	i.min = min;
	i.max = max;
	return (i);
}

/*
 * Returns the size of the interval
 */
double	size(const t_interval *i)
{
	return (i->max - i->min);
}

/*
 * Will return true for values including boundaries
 */
bool	contains(const t_interval *i, double x)
{
	return (x >= i->min && x <= i->max);
}

/*
 * Will return true for values inside excluding boundaries
 */
bool	surrounds(const t_interval *i, double x)
{
	return (x > i->min && x < i->max);
}
