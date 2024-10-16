/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   triangle2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ralgaran <ralgaran@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 16:20:09 by lbrusa            #+#    #+#             */
/*   Updated: 2024/10/14 17:36:04 by ralgaran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "color.h"
#include "material.h"
#include "texture.h"
#include "triangle.h"
#include "utils.h"
#include "vec3.h"
#include <stdio.h>

/**
 * @brief Calculate the probability density function value for the triangle.
 */
double	triangle_pdf_value(const void *self, const t_point3 *orig,
		const t_vec3 *dir)
{
	t_hit_record	rec;
	const t_ray		r = ray(*orig, *dir);
	t_vec3			e1;
	t_vec3			e2;
	double			area;

	if (!hit_triangle((t_triangle *)self, &r, interval(0.001, 1e30), &rec))
		return (0);
	e1 = vec3substr(((t_triangle *)self)->b, ((t_triangle *)self)->a);
	e2 = vec3substr(((t_triangle *)self)->c, ((t_triangle *)self)->a);
	area = 0.5 * length(cross(e1, e2));
	return (len_sqrd(vec3substr(rec.p, *orig)) / (fabs(dot(*dir,
					((t_triangle *)self)->normal)) * area));
}

t_vec3	triangle_random(const void *self, const t_point3 *orig)
{
	const t_triangle	*tri = (t_triangle *)self;
	double				u;
	double				v;
	t_vec3				p;

	u = random_d();
	v = random_d();
	if (u + v > 1)
	{
		u = 1 - u;
		v = 1 - v;
	}
	p = vec3add(tri->a, vec3add(vec3multscalar(vec3substr(tri->b, tri->a), u),
				vec3multscalar(vec3substr(tri->c, tri->a), v)));
	return (vec3substr(p, *orig));
}
