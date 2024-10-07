/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 12:06:24 by lbrusa            #+#    #+#             */
/*   Updated: 2024/10/07 17:34:30 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "color.h"
#include "interval.h"
#include "texture.h"
#include "utils.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

/**
 * @brief Initialize a solid color texture with the given color
 */
void	solid_color_init(t_solid_color *solid_color_texture, t_color albedo)
{
	solid_color_texture->base.value = solid_color_value;
	solid_color_texture->color_albedo = albedo;
}

/**
 * @brief Get the color of the texture at the given coordinates
 */
t_color	solid_color_value(const void *self, double uv[2], const t_point3 *p)
{
	t_solid_color	*solid_color;

	(void)uv;
	(void)p;
	solid_color = (t_solid_color *)self;
	return (solid_color->color_albedo);
}

/**
 * @brief - Checker texture
 * colors for the checkerboard ex
 * even_color = color(0.5, 0.0, 0.5); // Purple
 * odd_color = color(1.0, 1.0, 1.0); // White
 */
void	checker_texture_init(t_checker_texture *checker_texture, double scale,
		t_rgb even_rgb, t_rgb odd_rgb)
{
	t_color	even_color;
	t_color	odd_color;

	checker_texture->base.value = checker_texture_value;
	checker_texture->inv_scale = 1.0 / scale;
	even_color = rgb_to_color(even_rgb);
	odd_color = rgb_to_color(odd_rgb);
	solid_color_init(&checker_texture->even, even_color);
	solid_color_init(&checker_texture->odd, odd_color);
}

/**
 * @brief Get the spherical uv coordinates
 *
 * @param p The point in 3D space
 * @param u The u texture coordinate
 * @param v The v texture coordinate
 * Used in the sphere texture mapping. I take the intersection point
 * mapped to the unit sphere and return the spherical coordinates
 * of the point which will be used to get the color of the texture
 * in the accompanying image texture.
 */
void	get_spherical_uv(const t_point3 *p, double uv[2])
{
	double	phi;
	double	theta;

	phi = atan2(p->z, p->x);
	theta = acos(p->y);
	uv[0] = (phi + PI) / (2 * PI);
	uv[1] = theta / PI;
}

/**
 * @brief Get the color of the texture at the given coordinates
 *
 * @param self The texture object
 * @param u The u texture coordinate unused because we are using spherical
 * solid coordinates
 * @param v The v texture coordinate (unused see above)
 * @param p The point in 3D space
 * @return t_color The color of the texture at point of intersection
 * imagining the sphere build as solid blocks of color
 */
t_color	checker_texture_value(const void *self, double uv[2], const t_point3 *p)
{
	int		xint;
	int		yint;
	int		zint;
	bool	is_even;

	(void)uv;
	xint = (int)floor(p->x * ((t_checker_texture *)self)->inv_scale);
	yint = (int)floor(p->y * ((t_checker_texture *)self)->inv_scale);
	zint = (int)floor(p->z * ((t_checker_texture *)self)->inv_scale);
	is_even = (xint + yint + zint) % 2 == 0;
	if (is_even)
		return (((t_checker_texture *)self)->even.color_albedo);
	else
		return (((t_checker_texture *)self)->odd.color_albedo);
}
