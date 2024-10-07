/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quad2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 17:25:05 by lbrusa            #+#    #+#             */
/*   Updated: 2024/10/07 17:25:19 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "color.h"
#include "hittable.h"
#include "quad.h"
#include "utils.h"
#include <stdio.h>

double	quad_pdf_value(const void *self, const t_point3 *orig,
		const t_vec3 *dir)
{
	const t_quad	*qd = (t_quad *)self;
	t_hit_record	rec;
	const t_ray		r = ray(*orig, *dir);
	double			distance_squared;
	double			cosine;

	if (!hit_quad(qd, &r, interval(0.001, 1e30), &rec))
		return (0);
	distance_squared = len_sqrd(*dir) * rec.t * rec.t;
	cosine = (fabs(dot(*dir, rec.normal))) / length(*dir);
	return (distance_squared / (cosine * qd->area));
}

t_vec3	quad_random(const void *self, const t_point3 *orig)
{
	const t_quad	*qd = (t_quad *)self;
	t_vec3			p;

	p = vec3add(qd->q, vec3add(vec3multscalar(qd->u, random_double(0, 1)),
				vec3multscalar(qd->v, random_double(0, 1))));
	return (vec3substr(p, *orig));
}