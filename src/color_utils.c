/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 09:44:47 by lbrusa            #+#    #+#             */
/*   Updated: 2024/10/05 10:26:49 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "color.h"
#include "utils.h"

/**
 * @brief Convert a linear component to gamma
 */
double	linear_to_gamma(double linear_component)
{
	if (linear_component > 0)
		return (sqrt(linear_component));
	return (0);
}

/**
 * @brief Generate a random color
 */
t_color	color_random(void)
{
	return (color(random_d(), random_d(), random_d()));
}

/**
 * @brief Generate a random color with components between min and max
 */
t_color	color_random_min_max(double min, double max)
{
	return (color(random_double(min, max), random_double(min, max),
			random_double(min, max)));
}
