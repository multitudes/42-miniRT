/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3_utils3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 18:41:55 by lbrusa            #+#    #+#             */
/*   Updated: 2024/10/03 11:59:19 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"
#include "vec3.h"

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
		if (len_sqrd(p) < 1)
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
		if (len_sqrd(p) < 1)
			return (p);
	}
}

/**
 * @brief Random vector on hemisphere
 * @param normal normal vector
 * @return t_vec3
 *
 * returns a random unit vector in the same
 * hemisphere as the normal
 */
t_vec3	random_on_hemisphere(const t_vec3 normal)
{
	t_vec3	on_unit_sphere;

	on_unit_sphere = random_unit_vector();
	if (dot(on_unit_sphere, normal) > 0.0)
		return (on_unit_sphere);
	else
		return (vec3negate(on_unit_sphere));
}
