/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 10:34:22 by lbrusa            #+#    #+#             */
/*   Updated: 2024/10/09 10:52:57 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "color.h"
#include "cylinder.h"
#include "material.h"
#include "ray.h"
#include "utils.h"
#include "vec3.h"
#include <stdio.h>

/**
 * @brief: Computes the PDF value for a uniform cylinder.
 * @param: self: Pointer to the object
 * @param: orig: Pointer to the t_point3 origin vector
 * @param: dir: Pointer to the t_vec3 direction vector
 * @return: A double representing the PDF value for a uniform cylinder.
 *
 * The PDF value is calculated by projecting the ray direction onto the
 * cylinder axis. So we calculate the distance squared from origin to the
 * cylinder axis.
 * axis_distance_squared could be zero and we prevent division by zero.
 * We Project the ray direction onto the cylinder axis. We Calculate the
 * solid angle of the cylinder's surface visible from the origin.
 * Then return the PDF (inverse of the solid angle).
 */
double	obj_cylinder_pdf_value(const void *self, const t_point3 *orig,
		const t_vec3 *dir)
{
	const t_cylinder	*cyl = (t_cylinder *)self;
	t_vec3				axis_vector;
	double				axis_distance_squared;
	double				cos_theta;
	double				solid_angle;

	axis_vector = vec3substr(cyl->center, *orig);
	axis_distance_squared = len_sqrd(axis_vector);
	if (axis_distance_squared <= EPSILON)
		return (0);
	cos_theta = dot(axis_vector, *dir) / sqrt(axis_distance_squared);
	solid_angle = 2 * PI * cyl->radius * (1.0 - sqrt(1.0 - cos_theta
				* cos_theta));
	return (1.0 / solid_angle);
}

/**
 * @brief: Generates a random point on the surface of a cylinder.
 * @param: self: Pointer to the object
 * @param: orig: Pointer to the t_point3 origin vector
 * @return: A t_vec3 representing a random point on the surface of a cylinder.
 *
 * We generate a random point on the cylinder's surface
 * Translate the point to the cylinder's center.
 * Calculate the direction vector from the origin to the point on the cylinder.
 * Normalize the direction vector.
 */
t_vec3	obj_cylinder_random(const void *self, const t_point3 *orig)
{
	const t_cylinder	*cyl = (t_cylinder *)self;
	double				r1;
	double				r2;
	t_vec3				point_on_cylinder;
	t_vec3				direction;

	r1 = random_d();
	r2 = random_d();
	point_on_cylinder = vec3add(cyl->center, vec3(cyl->radius * cos(2.0 * PI
					* r1) * cos(r2), cyl->radius * sin(r2), cyl->radius
				* sin(2.0 * PI * r1) * cos(r2)));
	direction = vec3add(point_on_cylinder, *orig);
	return (unit_vector(direction));
}

/**
 * @brief: Computes the UV coordinates for a point on a uncapped cylinder.
 *
 * @param: normal: The normal vector of the point on the cylinder.
 * @param: uv: The UV coordinates to be calculated.
 *
 * Calculate theta (angle around the cylinder's axis).
 * Calculate phi (angle along the cylinder's height).
 * Map theta and phi to UV coordinates.
 */
void	get_cylinder_uncappedv(t_vec3 normal, double uv[2])
{
	double	theta;
	double	phi;

	theta = atan2(normal.x, normal.z);
	phi = acos(normal.y);
	uv[0] = theta / (2 * PI);
	uv[1] = phi / PI;
}
