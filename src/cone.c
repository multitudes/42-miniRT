/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ralgaran <ralgaran@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 16:06:09 by ralgaran          #+#    #+#             */
/*   Updated: 2024/09/16 16:06:10 by ralgaran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cone.h"
#include <math.h>

void	cone(t_cone *c, t_point3 apex, t_vec3 axis, double angle, double height, \
	t_rgb rgbcolor)
{
	c->base.hit = hit_cone;
	c->apex = apex;
	c->axis = axis;
	c->angle = angle;			// in degrees (half angle - max 89 deg)
	c->height = height;
	c->rgbcolor = rgbcolor;

	// TODO: solid color init???
}

// bool	hit_cone(const void *self, const t_ray *ray, t_interval closest, t_hit_record *rec)
// {
// 	const t_cone *c = (t_cone *)self;
// 	double a =

// 	// TODO: https://www.shadertoy.com/view/MtcXWr

// 	double b =
// 	double c =

// }
