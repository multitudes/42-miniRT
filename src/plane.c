/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 14:59:39 by lbrusa            #+#    #+#             */
/*   Updated: 2024/09/06 15:15:57 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "plane.h"


t_plane		plane(t_point3 center, t_vec3 normal, t_rgb color)
{
	t_plane result;

	result.base.hit = hit_plane;
	result.point = center;
	result.normal = normal;
	result.rgb = color;
	result.print = &print_plane;
	return result;


}

/**
 * @brief print the plane object
 * 
 * format is like this 
 * pl 0.0,0.0,-10.0 0.0,1.0,0.0 0,0,225
 */
void		print_plane(const void *self)
{
	const t_plane *p = (const t_plane *)self;
	printf("sp\t%.f,%.f,%.f\t\t%.f,%.f,%.f\t\t\t%d,%d,%d\n", 
	p->point.x, p->point.y, p->point.z, p->normal.x, p->normal.y, p->normal.z, p->rgb.r, p->rgb.g, p->rgb.b);

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
