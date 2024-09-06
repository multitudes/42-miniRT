/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 15:07:07 by lbrusa            #+#    #+#             */
/*   Updated: 2024/09/06 15:17:27 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cylinder.h"
#include "vec3.h"
#include "ray.h"
#include "color.h"

t_cylinder		cylinder(t_point3 center, t_vec3 axis, double diameter, double height, t_rgb color)
{
	t_cylinder result;

	result.base.hit = hit_cyclinder;
	result.center = center;
	result.axis = axis;
	result.radius = diameter / 2;
	result.height = height;
	result.rgb = color;
	result.print = &print_cylinder;
	return result;
}

/**
 * @brief print the cylinder object
 * cy 50.0,0.0,20.6 	 0.0,0.0,1.0  	14.2	 21.42  	10,0,255
 */
void		print_cylinder(const void *self)
{
	const t_cylinder *c = (const t_cylinder *)self;
	printf("sp\t%.f,%.f,%.f\t\t%.f,%.f,%.f\t\t%f\t%f\t%d,%d,%d\n", 
	c->center.x, c->center.y, c->center.z, c->axis.x, c->axis.y, c->axis.z, c->radius * 2, c->height, c->rgb.r, c->rgb.g, c->rgb.b);

}


bool		hit_cyclinder(const void* self, const t_ray *r, t_interval closest, t_hit_record *rec)
{
	(void)self;
	(void)r;
	(void)closest;
	(void)rec;
	return false;
}