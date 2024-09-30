/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 16:06:09 by ralgaran          #+#    #+#             */
/*   Updated: 2024/09/30 09:59:18 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cone.h"
#include "debug.h"
#include <math.h>

void	cone(t_cone *c, t_point3 apex, t_vec3 axis, double angle, double height,
		t_rgb rgbcolor)
{
	c->base.hit = hit_cone;
	c->apex = apex;
	c->axis = axis;
	c->angle = angle / 2; // in degrees (half angle - max 89 deg)
	c->height = height;
	c->rgbcolor = rgbcolor;
	// TODO: solid color init???
}

bool	hit_cone(const void *self, const t_ray *ray, t_interval closest,
		t_hit_record *rec)
{
	const t_cone	*cone = (t_cone *)self;
	t_vec3			co;
	double			a;
	double			b;
	double			c;
	double			discr;
	double			squared;
	double			root;

	co = vec3substr(ray->orig, cone->apex);
	a = pow(dot(ray->dir, cone->axis), 2) - pow(cos(cone->angle), 2);
	b = 2 * (dot(ray->dir, cone->axis) * dot(co, cone->axis) - (dot(ray->dir,
					co) * pow(cos(cone->angle), 2)));
	c = pow(dot(co, cone->axis), 2) - (dot(co, co) * pow(cos(cone->angle), 2));
	discr = b * b - 4. * a * c;
	if (discr < 0)
		return (false);
	squared = sqrt(discr);
	root = (b - squared) / a;
	if (!surrounds(&closest, root))
	{
		root = (b + squared) / a;
		if (!surrounds(&closest, root))
			return (false);
	}
	rec->t = root;
	rec->p = point_at(ray, root);
	debug("hit the cone");
	// TODO: i need the point and the normal - need to populate hit record
	return (true);
}
