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

/**
 * @brief Initialize a point in 3D space
 */
t_point3	point3(double x, double y, double z)
{
	t_point3	result;

	result.x = x;
	result.y = y;
	result.z = z;
	return (result);
}

/**
 * @brief Negate a vector
 */
t_vec3	vec3negate(const t_vec3 a)
{
	t_vec3	result;

	result.x = -a.x;
	result.y = -a.y;
	result.z = -a.z;
	return (result);
}

/**
 * @brief Add two vectors
 */
t_vec3	vec3add(const t_vec3 a, const t_vec3 b)
{
	t_vec3	result;

	result.x = a.x + b.x;
	result.y = a.y + b.y;
	result.z = a.z + b.z;
	return (result);
}

/**
 * @brief Subtract two vectors
 */
t_vec3	vec3substr(const t_vec3 a, const t_vec3 b)
{
	t_vec3	result;

	result.x = a.x - b.x;
	result.y = a.y - b.y;
	result.z = a.z - b.z;
	return (result);
}
