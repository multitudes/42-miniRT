/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 15:46:09 by lbrusa            #+#    #+#             */
/*   Updated: 2024/10/10 19:36:21 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

/*
 * @brief Comverts degrees to radians.
 */
double	degrees_to_radians(double degrees)
{
	return (degrees * PI / 180.0);
}

/*
 * @brief Returns a random int in [min,max).
 *
 * max is excluded.
 */
int	random_int(int min, int max)
{
	int	diff;

	diff = max - min;
	if (diff <= 0)
		diff = 1;
	return (min + rand_rt() % diff);
}

/*
 * @brief Returns a random real in [0,1), 1 excluded.
 */
double	random_d(void)
{
	return (rand_rt() / (UINT32_MAX + 1.0));
}

/*
 * @brief Returns a random real in [min,max)
 * and max excluded.
 */
double	random_double(double min, double max)
{
	return (min + (max - min) * random_d());
}

t_vec3	clamp_color(t_vec3 v)
{
	return (vec3divscalar(v, clamp(interval(1, length(v)), length(v))));
}
