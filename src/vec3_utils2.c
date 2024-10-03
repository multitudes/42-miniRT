/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 18:37:29 by lbrusa            #+#    #+#             */
/*   Updated: 2024/10/03 15:37:54 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"
#include "vec3.h"

/*
 * @brief dot product of vectors
 * a . b = a1*b1 + a2*b2 + a3*b3
 */
double	dot(const t_vec3 a, const t_vec3 b)
{
	double	result;

	result = a.x * b.x + a.y * b.y + a.z * b.z;
	return (result);
}

/*
 * @brief Cross product of vectors
 *returns the cross product of a and b by value
 */
t_vec3	cross(const t_vec3 a, const t_vec3 b)
{
	t_vec3	result;

	result.x = a.y * b.z - a.z * b.y;
	result.y = a.z * b.x - a.x * b.z;
	result.z = a.x * b.y - a.y * b.x;
	return (result);
}

/**
 * @brief Get a random vector
 * Components are random doubles between 0 and 1
 */
t_vec3	random_vec3(void)
{
	return (vec3(random_d(), random_d(), random_d()));
}

/**
 * @brief Get a random vector with components between min and max
 */
t_vec3	random_vec3_min_max(double min, double max)
{
	return (vec3(random_double(min, max), random_double(min, max),
			random_double(min, max)));
}

/**
 * @brief Check if a vector is close to zero
 *
 * @param v the vector to check
 * @return true if the vector is close to zero in all dimensions
 * 
 * I use a near zero value of 1e-8 as a double precision floating point
 */
bool	near_zero(t_vec3 e)
{
	const double	s = 1e-8;

	return ((fabs(e.x) < s) && (fabs(e.y) < s) && (fabs(e.z) < s));
}
