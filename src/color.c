/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 14:49:03 by lbrusa            #+#    #+#             */
/*   Updated: 2024/09/06 11:49:38 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include "color.h"
#include "utils.h"

/**
 * @brief Create a color struct with each component 
 * between 0 and 1
 */
t_color color(double r, double g, double b)
{
	t_color result;

	result.r = r;
	result.g = g;
	result.b = b;
	return result;
}

/**
 * @brief Create a rgb struct to use in the initializer
 * for the sphere and other objects
 */
t_rgb rgb(int r, int g, int b)
{
	t_rgb rgbcolor;

	rgbcolor.r = r;
	rgbcolor.g = g;
	rgbcolor.b = b;
	return rgbcolor;
}

/**
 * @brief Convert a linear component to gamma
 */
double linear_to_gamma(double linear_component)
{
    if (linear_component > 0)
        return sqrt(linear_component);

    return 0;
}

/**
 * @brief Generate a random color
 */
t_color		color_random()
{
	return color(random_double(0, 1), random_double(0, 1), random_double(0, 1));
}

/**
 * @brief Generate a random color with components between min and max
 */
t_color		color_random_min_max(double min, double max)
{
	return color(random_double(min, max), random_double(min, max), random_double(min, max));
}
