/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 18:34:54 by lbrusa            #+#    #+#             */
/*   Updated: 2024/10/02 18:37:15 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"
#include "vec3.h"

/**
 * @brief Multiply two vectors component-wise
 */
t_vec3	vec3mult(const t_vec3 a, const t_vec3 b)
{
	t_vec3	result;

	result.x = a.x * b.x;
	result.y = a.y * b.y;
	result.z = a.z * b.z;
	return (result);
}

/**
 * @brief Multiply a vector by a scalar
 */
t_vec3	vec3multscalar(const t_vec3 a, double t)
{
	t_vec3	result;

	result.x = a.x * t;
	result.y = a.y * t;
	result.z = a.z * t;
	return (result);
}

/**
 * @brief Divide a vector by a scalar
 */
t_vec3	vec3divscalar(const t_vec3 a, double t)
{
	t_vec3	result;

	result.x = a.x / t;
	result.y = a.y / t;
	result.z = a.z / t;
	return (result);
}

/**
 * @brief get the squared length of a vector
 * Used for normalization
 */
double	length_squared(const t_vec3 v)
{
	return (v.x * v.x + v.y * v.y + v.z * v.z);
}

/**
 * @brief get the length of a vector
 */
double	length(const t_vec3 v)
{
	return (sqrt(length_squared(v)));
}
