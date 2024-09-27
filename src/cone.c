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
#include "debug.h"

void	cone_rgb(t_cone *c, t_point3 apex, t_vec3 axis, double angle, double height, \
	t_rgb rgbcolor)
{
	c->base.hit = hit_cone;
	c->apex = apex;
	c->axis = axis;
	c->angle = angle / 2;			// in degrees (half angle - max 89 deg)
	c->height = height;
	c->color = rgb_to_color(rgbcolor);
	solid_color_init(&(c->texture), c->color);
	lambertian_init_tex(&(c->lambertian_mat), (t_texture*)&(c->texture));
	c->mat = (t_material*)&(c->lambertian_mat);
}

void	cone_mat(t_cone *c, t_point3 apex, t_vec3 axis, double angle, double height, \
	t_material *mat)
{
	c->base.hit = hit_cone;
	c->apex = apex;
	c->axis = axis;
	c->angle = angle / 2;			// in degrees (half angle - max 89 deg)
	c->height = height;
	c->mat = mat;
}
bool	hit_cone(const void *self, const t_ray *ray, t_interval closest, t_hit_record *rec)
{

	return true;
}
