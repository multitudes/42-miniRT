/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 13:54:19 by lbrusa            #+#    #+#             */
/*   Updated: 2024/10/03 15:01:44 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "plane.h"
#include "utils.h"

/**
 * @brief plane_pdf_value - Computes the PDF value for a plane
 * 
 * @param self the plane object
 * @param orig the origin point
 * @param dir the direction vector
 * 
 * The function plane_pdf_value computes the probability density function (PDF)
 * value for a plane. The PDF value is the reciprocal of the solid angle
 * determining the probability density function (PDF) value for sampling
 * directions towards a plane. We do not currently use this function in the
 * raytracer because we do not use the plane as a light source.
 * But if we would use it as a light source, we would get very little out of it
 * because the plane is infinite and the PDF value would be very small.
 * I divide at the end by 1e30 * cosine. 1e30 is a large number 
 * to represent the area of the plane.
 */
double	plane_pdf_value(const void *self, const t_point3 *orig,
		const t_vec3 *dir)
{
	const t_plane	*pl = (t_plane *)self;
	t_hit_record	rec;
	const t_ray		r = ray(*orig, *dir);
	double			distance_squared;
	double			cosine;

	if (!hit_plane(pl, &r, interval(0.001, 1e30), &rec))
		return (0);
	distance_squared = len_sqrd(vec3substr(rec.p, *orig));
	cosine = fabs(dot(*dir, pl->normal));
	if (cosine < 1e-8)
		return (0);
	return (distance_squared / (cosine * 1e30));
}

/**
 * @brief plane_random - Get a random vector on the plane
 * 
 * We do not currently use this function in the
 * raytracer because we do not use the plane as a light source.
 */
t_vec3	plane_random(const void *self, const t_point3 *orig)
{
	const t_plane	*pl = (t_plane *)self;
	double			max_u;
	double			max_v;
	t_vec3			p;

	max_u = random_d() * PLANE_MAX;
	max_v = random_d() * PLANE_MAX;
	p = vec3add(pl->q, vec3add(vec3multscalar(pl->u, max_u),
				vec3multscalar(pl->v, max_v)));
	return (vec3substr(p, *orig));
}
