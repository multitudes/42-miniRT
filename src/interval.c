/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interval.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 09:59:43 by lbrusa            #+#    #+#             */
/*   Updated: 2024/09/05 13:11:44 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "interval.h"

/*
 * initializer
 */
t_interval interval(double min, double max)
{
	t_interval i;

	i.min = min;
	i.max = max;
	return (i);
}

/*
 * Returns the size of the interval
 */
double size(const t_interval *i)
{
	return (i->max - i->min);
}

/*
 * Will return true for values including boundaries
 */
bool contains(const t_interval *i, double x)
{
	return (x >= i->min && x <= i->max);
}

/*
 * Will return true for values inside excluding boundaries
 */
bool surrounds(const t_interval *i, double x)
{
	return (x > i->min && x < i->max);
}

/*
 * Returns an empty interval
 */
t_interval empty_interval()
{
	return (t_interval){0, 0};
}

/*
 * Returns an interval that contains all values
 */
t_interval universe_interval()
{
	return (t_interval){-INFINITY, INFINITY};
}

double	clamp(t_interval t, double x)
{
	if (x < t.min) return t.min;
	if (x > t.max) return t.max;
	return x;
}
