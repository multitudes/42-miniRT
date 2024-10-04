/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 10:32:50 by lbrusa            #+#    #+#             */
/*   Updated: 2024/10/03 11:59:19 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "hittable.h"
#include "interval.h"
#include "ray.h"
#include "sphere.h"
#include "utils.h"
#include "vec3.h"

/**
 * @brief set face normal
 *
 * @param rec the hit record
 * @param r the ray
 * @param outward_normal the outward normal
 *
 * This function sets the face normal of the hit record
 * based on the ray and the outward normal we can determine
 * if the ray is hitting the front or the back of the object
 */
void	set_face_normal(t_hit_record *rec, const t_ray *r,
		const t_vec3 outward_normal)
{
	rec->front_face = dot(r->dir, outward_normal) < 0;
	if (rec->front_face)
		rec->normal = outward_normal;
	else
		rec->normal = vec3negate(outward_normal);
}

/*
 * @brief get_sphere_uv
 *
 * @param normal the normal vector
 * @param uv the uv coordinates to get populated
 *
 * The uv coordinates are a way to map a 3D point
 * on the surface of a sphere to a 2D coordinate system
 *
 */
void	get_sphere_uv(t_vec3 normal, double uv[2])
{
	double	theta;
	double	phi;

	theta = acos(-normal.y);
	phi = atan2(-normal.z, normal.x) + PI;
	uv[0] = phi / (2 * PI);
	uv[1] = theta / PI;
}

/**
 * sphere_pdf_value - Computes the PDF value for a uniform sphere.
 * @self: Pointer to the object
 * @direction: Pointer to the t_vec3 direction vector
 *
 * This function returns the probability density function (PDF) value for a
 * uniformly distributed direction over the surface of a sphere. 
 * determining the probability density function (PDF) value for sampling 
 * directions towards a light source or an object. 
 * 
 * Since the distribution is uniform, the PDF value is constant and equal 
 * to the inverse of the surface area of the sphere, which is 1 / (4 * PI).
 *
 * Return: A double representing the PDF value for a uniform sphere.
 * The solid angle is calculated to determine the fraction of the total 
 * sphere's surface area that is visible from the point of origin. 
 * The solid angle represents the "spread" of the directions that 
 * intersect the object from a given point of view.
 * The solid angle Ω is a measure of how large an object 
 * appears to an observer from a given point. It is measured in steradians (sr).
 * Cosine of Maximum Angle: cos_theta_max represents the cosine of the maximum 
 * angle θ_max that a ray can make with the center of the sphere and still 
 * intersect the sphere. The PDF value is the reciprocal of the solid angle
 */
double	obj_sphere_pdf_value(const void *self, const t_point3 *orig,
		const t_vec3 *dir)
{
	const t_sphere	*s = (t_sphere *)self;
	t_hit_record	rec;
	const t_ray		r = ray(*orig, *dir);
	double			cos_theta_max;
	double			solid_angle;

	if (!hit_sphere(s, &r, interval(0.001, 1e30), &rec))
		return (0);
	cos_theta_max = sqrt(1.0 - (s->radius * s->radius / \
					(len_sqrd(vec3substr(s->center, *orig)))));
	solid_angle = 2.0 * PI * (1.0 - cos_theta_max);
	return (1.0 / solid_angle);
}

/**
 * @brief obj_sphere_random
 *
 * @param self the sphere object
 * @param orig the origin point
 *
 * The function obj_sphere_random generates a random point within a sphere 
 * and returns the transformed point. This point is used in the context of 
 * sampling directions towards a spherical light source.
 * To calculate the lighting contribution 
 * you need to sample multiple directions towards the light source.
 * The function generates a random point on the surface of the sphere, 
 * which represents a potential direction towards the light source.
 * The random point is transformed from the local coordinate system 
 * to the world coordinate system
 */
t_vec3	obj_sphere_random(const void *self, const t_point3 *orig)
{
	const t_sphere	*s = (t_sphere *)self;
	t_vec3			dir_to;
	double			distance_squared;
	t_onb			uvw;
	t_vec3			random_point;

	dir_to = vec3substr(s->center, *orig);
	distance_squared = len_sqrd(dir_to);
	onb_build_from_w(&uvw, &dir_to);
	random_point = random_to_sphere(s->radius, distance_squared);
	return (onb_transform(&uvw, random_point));
}

/**
 * @brief random_to_sphere
 * 
 * @param radius the radius of the sphere
 * @param distance_squared the distance squared
 * 
 * The function random_to_sphere generates a random direction within 
 * the volume of a sphere. This function is particularly useful in 
 * the context of light sampling for rendering, where you need to 
 * sample directions towards a spherical light source to calculate 
 * its contribution to the scene's illumination.
 * The term sqrt(1.0 - radius * radius / distance_squared) ensures 
 * that the point lies within the sphere's volume.
 */
t_vec3	random_to_sphere(double radius, double distance_squared)
{
	double	r[2];
	double	z;
	double	phi;
	double	x;
	double	y;

	r[0] = random_d();
	r[1] = random_d();
	z = 1.0 + r[1] * (sqrt(1.0 - radius * radius / distance_squared) - 1.0);
	phi = 2.0 * PI * r[0];
	x = cos(phi) * sqrt(1.0 - z * z);
	y = sin(phi) * sqrt(1.0 - z * z);
	return (unit_vector(vec3(x, y, z)));
}
