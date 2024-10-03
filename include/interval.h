/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interval.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 09:53:54 by lbrusa            #+#    #+#             */
/*   Updated: 2024/09/30 09:39:11 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INTERVAL_H
# define INTERVAL_H

# include <math.h>
# include <stdbool.h>

/*
 * Used to represent a range of t values
 */
typedef struct s_interval
{
	double	min;
	double	max;
}			t_interval;

/*
 * Initializer
 */
t_interval	interval(double min, double max);

double		size(const t_interval *i);
bool		contains(const t_interval *i, double x);
bool		surrounds(const t_interval *i, double x);
t_interval	empty_interval(void);
t_interval	universe_interval(void);
double		clamp(t_interval t, double x);

#endif
