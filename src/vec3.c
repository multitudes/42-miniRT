/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 14:39:04 by lbrusa            #+#    #+#             */
/*   Updated: 2024/06/19 13:16:53 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"
#include "vec3.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

/*
 * This is the C equivalent of a C++ initializer
 */
t_vec3	vec3(double x, double y, double z)
{
	t_vec3	result;

	result.x = x;
	result.y = y;
	result.z = z;
	return (result);
}

t_point3	point3(double x, double y, double z)
{
	t_point3	result;

	result.x = x;
	result.y = y;
	result.z = z;
	return (result);
}

t_vec3	vec3negate(const t_vec3 a)
{
	t_vec3	result;

	result.x = -a.x;
	result.y = -a.y;
	result.z = -a.z;
	return (result);
}

t_vec3	vec3add(const t_vec3 a, const t_vec3 b)
{
	t_vec3	result;

	result.x = a.x + b.x;
	result.y = a.y + b.y;
	result.z = a.z + b.z;
	return (result);
}

t_vec3	vec3substr(const t_vec3 a, const t_vec3 b)
{
	t_vec3	result;

	result.x = a.x - b.x;
	result.y = a.y - b.y;
	result.z = a.z - b.z;
	return (result);
}

t_vec3	vec3mult(const t_vec3 a, const t_vec3 b)
{
	t_vec3	result;

	result.x = a.x * b.x;
	result.y = a.y * b.y;
	result.z = a.z * b.z;
	return (result);
}

t_vec3	vec3multscalar(const t_vec3 a, double t)
{
	t_vec3	result;

	result.x = a.x * t;
	result.y = a.y * t;
	result.z = a.z * t;
	return (result);
}

t_vec3	vec3divscalar(const t_vec3 a, double t)
{
	t_vec3	result;

	result.x = a.x / t;
	result.y = a.y / t;
	result.z = a.z / t;
	return (result);
}

double	length_squared(const t_vec3 v)
{
	return (v.x * v.x + v.y * v.y + v.z * v.z);
}

double	length(const t_vec3 v)
{
	return (sqrt(length_squared(v)));
}

// Vector Utility Functions
/*
 * to debug
 */
void	print_vec3(const t_vec3 *v)
{
	printf("%f %f %f\n", v->x, v->y, v->z);
}

/*
 * dot product of vectors a . b = a1*b1 + a2*b2 + a3*b3
 */
double	dot(const t_vec3 a, const t_vec3 b)
{
	double	result;

	result = a.x * b.x + a.y * b.y + a.z * b.z;
	return (result);
}

/*
returns the cross product of a and b by value
*/
t_vec3	cross(const t_vec3 a, const t_vec3 b)
{
	t_vec3	result;

	result.x = a.y * b.z - a.z * b.y;
	result.y = a.z * b.x - a.x * b.z;
	result.z = a.x * b.y - a.y * b.x;
	return (result);
}

t_vec3	random_vec3(void)
{
	return (vec3(random_d(), random_d(), random_d()));
}

t_vec3	random_vec3_min_max(double min, double max)
{
	return (vec3(random_double(min, max), random_double(min, max),
			random_double(min, max)));
}

bool	near_zero(t_vec3 e)
{
	const double	s = 1e-8;

	// Return true if the vector is close to zero in all dimensions.
	return (fabs(e.x) < s) && (fabs(e.y) < s) && (fabs(e.z) < s);
}

t_vec3	sample_square(void)
{
	t_vec3	random_vec;

	random_vec = vec3(random_d() - 0.5, random_d() - 0.5, 0);
	return (random_vec);
}

t_vec3	unit_vector(t_vec3 v)
{
	return (vec3divscalar(v, length(v)));
}

t_vec3	random_in_unit_disk(void)
{
	t_vec3	p;

	while (1)
	{
		p = vec3(random_double(-1, 1), random_double(-1, 1), 0);
		if (length_squared(p) < 1)
			return (p);
	}
}

/*
 * Random vector in unit sphere
 */
t_vec3	random_in_unit_sphere(void)
{
	t_vec3	p;

	while (1)
	{
		p = random_vec3_min_max(-1, 1);
		if (length_squared(p) < 1)
			return (p);
	}
}

t_vec3	random_on_hemisphere(const t_vec3 normal)
{
	t_vec3	on_unit_sphere;

	on_unit_sphere = random_unit_vector();
	if (dot(on_unit_sphere, normal) > 0.0)
		// In the same hemisphere as the normal
		return on_unit_sphere;
	else
		return vec3negate(on_unit_sphere);
}

t_vec3	random_unit_vector(void)
{
	return unit_vector(random_in_unit_sphere());
}

t_vec3	reflect(const t_vec3 v, const t_vec3 n)
{
	return vec3substr(v, vec3multscalar(n, dot(v, n) * 2));
}

t_vec3	refract(const t_vec3 uv, const t_vec3 n, double etai_over_etat)
{
	double	cos_theta;
	t_vec3	r_out_perp;
	t_vec3	r_out_parallel;

	cos_theta = fmin(dot(vec3negate(uv), n), 1.0);
	r_out_perp = vec3multscalar(vec3add(uv, vec3multscalar(n, cos_theta)),
			etai_over_etat);
	r_out_parallel = vec3multscalar(n, -sqrt(fabs(1.0
					- length_squared(r_out_perp))));
	return vec3add(r_out_perp, r_out_parallel);
}

double	reflectance(double cosine, double refraction_index)
{
	double	r0;

	// Use Schlick's approximation for reflectance.
	r0 = (1 - refraction_index) / (1 + refraction_index);
	r0 = r0 * r0;
	return r0 + (1 - r0) * pow((1 - cosine), 5);
}

t_vec3	random_cosine_direction(void)
{
	double r1 = random_d();
	double r2 = random_d();

	double phi = 2 * PI * r1;
	double x = cos(phi) * sqrt(r2);
	double y = sin(phi) * sqrt(r2);
	double z = sqrt(1 - r2);

	return vec3(x, y, z);
}