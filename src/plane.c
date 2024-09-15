/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 14:59:39 by lbrusa            #+#    #+#             */
/*   Updated: 2024/09/06 20:36:31 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "plane.h"

void	plane(t_plane *result, t_point3 center, t_vec3 normal, t_rgb color)
{
	result->base.hit = hit_plane;
	result->point = center;
	result->normal = normal;
	result->rgb = color;
}

/* if the ray hits the plane, return the t value */
bool		hit_plane(const void* self, const t_ray *r, t_interval closest, t_hit_record *rec)
{
	(void)self;
	(void)r;
	(void)closest;
	(void)rec;
	return false;
}
