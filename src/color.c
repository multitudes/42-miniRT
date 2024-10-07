/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 14:49:03 by lbrusa            #+#    #+#             */
/*   Updated: 2024/10/07 13:11:27 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "color.h"
#include "utils.h"
#include <limits.h>

/**
 * @brief Create a color struct with each component
 * between 0 and 1
 */
t_color	color(double r, double g, double b)
{
	t_color	result;

	result.r = r;
	result.g = g;
	result.b = b;
	return (result);
}

/**
 * @brief Create a rgb struct to use in the initializer
 * for the sphere and other objects
 */
t_rgb	rgb(uint32_t r, uint32_t g, uint32_t b)
{
	t_rgb	rgbcolor;

	rgbcolor.r = r;
	rgbcolor.g = g;
	rgbcolor.b = b;
	return (rgbcolor);
}

t_color	rgb_to_color(t_rgb rgbcolor)
{
	t_color	color;

	color.r = (double)rgbcolor.r / 255;
	color.g = (double)rgbcolor.g / 255;
	color.b = (double)rgbcolor.b / 255;
	return (color);
}

/**
 * @brief Convert a color to a rgb struct
 */
t_rgb	color_to_rgb(t_color color)
{
	t_rgb	rgb;

	rgb.r = (uint32_t)(255.999 * color.x);
	rgb.g = (uint32_t)(255.999 * color.y);
	rgb.b = (uint32_t)(255.999 * color.z);
	return (rgb);
}

unsigned int	rgb_to_uint(t_rgb rgbcolor)
{
	unsigned int	result;

	result = ((rgbcolor.r << 24) | (rgbcolor.g << 16) | (rgbcolor.b << 8) \
				| 0xFF);
	return (result);
}
