/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 15:20:34 by ralgaran          #+#    #+#             */
/*   Updated: 2024/10/14 17:01:32 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cone.h"

/*
 * Make a hittable list to pass into hit_objects.
 * The point is that hit_objects is going to call the hit function
 * of cylinder itself and the base.
*/
bool	hit_cone_cap(const void *self, const t_ray *r, t_interval closest, \
	t_hit_record *rec)
{
	const t_cone	*c = (t_cone *)self;
	t_hittablelist	cone_hit_list;
	t_hittable		*list[2];

	list[0] = (t_hittable *)(&c->body);
	list[1] = (t_hittable *)(&c->bottom);
	cone_hit_list = hittablelist(list, 2);
	return (hit_objects(&cone_hit_list, r, closest, rec));
}

/*
 * Calculates the pdf value of a cone
*/
double	obj_cone_pdf_value(const void *self, const t_point3 *orig, \
	const t_vec3 *dir)
{
	const t_cone	*cone;
	t_vec3			apex_to_orig_norm;
	double_t		cos_theta;
	double			half_angle_cos;

	(void) dir;
	cone = (t_cone *) self;
	apex_to_orig_norm = unit_vector(vec3substr(*orig, cone->body.apex));
	cos_theta = dot(cone->body.axis, apex_to_orig_norm);
	half_angle_cos = cone->body.height / sqrt(pow(cone->body.height, 2) + \
					pow(cone->body.radius, 2));
	if (cos_theta < half_angle_cos)
		return (0);
	return (1.0 / (2 * PI * (1 - half_angle_cos)));
}

/*
 * Returns a random point on the surface of a cone.
*/
t_point3	obj_cone_random(const void *self, const t_point3 *orig)
{
	const t_cone_uncap	*cone;
	double				phi;
	double				h;
	double				radius_at_height;
	t_point3			point_on_cone;

	cone = (t_cone_uncap *)self;
	phi = 2.0 * PI * random_d();
	h = random_d() * cone->height;
	radius_at_height = (cone->height - h) * cone->radius / cone->height;
	point_on_cone = vec3(
			radius_at_height * cos(phi),
			radius_at_height * sin(phi),
			h
			);
	point_on_cone = vec3add(point_on_cone, *orig);
	return (point_on_cone);
}
