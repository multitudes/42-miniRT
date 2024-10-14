/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   disk_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 15:04:24 by lbrusa            #+#    #+#             */
/*   Updated: 2024/10/14 16:42:12 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "debug.h"
#include "disk.h"
#include "hittable.h"
#include "minirt.h"
#include "utils.h"
#include <stdio.h>
#include "libft.h"

/**
 * @brief Computes the PDF value for a uniform disk.
 *
 * @param self the disk object
 * @param orig the origin point
 * @param dir the direction vector
 *
 * This function returns the probability density function (PDF) value for a
 * uniformly distributed direction over the surface of a disk.
 * This funbction is used in the context of sampling directions towards a disk
 * to calc the lighting contribution when the disk is used as a light source.
 * The PDF value is the reciprocal of the solid angle, therefore the PDF value
 * is divided by the area of the disk which is PI * radius * radius at the end.
 * Also it is inversely proportional to the cosine of the angle between the
 * direction vector and the normal vector of the disk.
 */
double	disk_pdf_value(const void *self, const t_point3 *orig,
		const t_vec3 *dir)
{
	t_hit_record	rec;
	const t_disk	*d = (t_disk *)self;
	const t_ray		r = ray(*orig, *dir);
	double			cos;
	double			radius;

	if (!hit_disk(d, &r, interval(0.001, 1e30), &rec))
		return (0);
	cos = fabs(dot(*dir, d->normal));
	radius = length(d->u);
	return (len_sqrd(vec3substr(rec.p, *orig)) / (cos * PI * radius * radius));
}

/**
 * @brief Get a random vector on the disk
 *
 * @param self the disk object
 * @param orig the origin point
 *
 * the orig point represents the origin point from which
 * we are sampling directions towards the disk. This is the cam
 * position in the raytracer. Used when the disk is used as a light
 * source.
 *
 */
t_vec3	disk_random(const void *self, const t_point3 *orig)
{
	const t_disk	*d = (t_disk *)self;
	double			r;
	double			theta;
	t_vec3			point_on_disk;
	t_vec3			direction;

	r = sqrt(random_double(0, 1));
	theta = 2 * PI * random_double(0, 1);
	point_on_disk = vec3add(d->center, vec3add(vec3multscalar(d->u, r
					* cos(theta)), vec3multscalar(d->v, r * sin(theta))));
	direction = vec3substr(point_on_disk, *orig);
	return (unit_vector(direction));
}
