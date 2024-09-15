/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 15:07:07 by lbrusa            #+#    #+#             */
/*   Updated: 2024/09/06 20:36:55 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cylinder.h"
#include "vec3.h"
#include "ray.h"
#include "color.h"
#include <stdio.h>

void	cylinder(t_cylinder *result, t_point3 center, t_vec3 axis, double diameter, double height, t_rgb color)
{
	result->base.hit = hit_cyclinder;
	result->center = center;
	result->axis = axis;
	result->radius = diameter / 2;
	result->height = height;
	result->rgb = color;
}

bool		hit_cyclinder(const void* self, const t_ray *r, t_interval closest, t_hit_record *rec)
{
	(void)self;
	(void)r;
	(void)closest;
	(void)rec;
	return false;
}
