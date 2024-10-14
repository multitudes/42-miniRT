/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quad.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 09:13:07 by lbrusa            #+#    #+#             */
/*   Updated: 2024/10/14 16:41:15 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "color.h"
#include "hittable.h"
#include "quad.h"
#include "utils.h"
#include <stdio.h>
#include "libft.h"

/**
 * @brief Initialize a quad object with a given position q,
	and two vectors u and v.
 */
void	quad_rgb(t_quad *qd, t_init_params params)
{
	t_vec3	n;

	qd->base.hit = hit_quad;
	qd->base.pdf_value = quad_pdf_value;
	qd->base.random = quad_random;
	qd->q = params.center;
	qd->u = params.side1;
	qd->v = params.side2;
	qd->rgb = params.rgbcolor;
	qd->color = rgb_to_color(params.rgbcolor);
	n = cross(params.side1, params.side2);
	qd->normal = unit_vector(n);
	qd->d = dot(qd->normal, params.center);
	qd->w = vec3divscalar(n, dot(n, n));
	qd->area = length(n);
	solid_color_init(&(qd->texture), qd->color);
	lambertian_init_tex(&(qd->lambertian_mat), (t_texture *)&(qd->texture));
	qd->mat = (t_material *)&(qd->lambertian_mat);
}

void	quad_mat(t_quad *qd, t_init_params params)
{
	t_vec3	n;

	qd->base.hit = hit_quad;
	qd->base.pdf_value = quad_pdf_value;
	qd->base.random = quad_random;
	qd->q = params.center;
	qd->u = params.side1;
	qd->v = params.side2;
	qd->rgb = params.rgbcolor;
	qd->color = rgb_to_color(params.rgbcolor);
	n = cross(params.side1, params.side2);
	qd->normal = unit_vector(n);
	qd->d = dot(qd->normal, params.center);
	qd->w = vec3divscalar(n, dot(n, n));
	qd->area = length(n);
	qd->mat = params.mat;
}

bool	hit_quad(const void *self, const t_ray *r, t_interval ray_t,
		t_hit_record *rec)
{
	const t_quad	*qd = (t_quad *)self;
	double			denom;
	t_vec3			planar_hitpt_vector;
	double			alpha;
	double			beta;

	denom = dot(qd->normal, r->dir);
	if (fabs(denom) < 1e-8)
		return (false);
	rec->t = (qd->d - dot(qd->normal, r->orig)) / denom;
	if (!contains(&ray_t, rec->t))
		return (false);
	planar_hitpt_vector = vec3substr(point_at(r, rec->t), qd->q);
	alpha = dot(qd->w, cross(planar_hitpt_vector, qd->v));
	beta = dot(qd->w, cross(qd->u, planar_hitpt_vector));
	if (!is_interior(alpha, beta, rec))
		return (false);
	rec->p = point_at(r, rec->t);
	rec->mat = qd->mat;
	set_face_normal(rec, r, qd->normal);
	return (true);
}

bool	is_interior(double a, double b, t_hit_record *rec)
{
	t_interval	unit_interval;

	unit_interval = interval(0, 1);
	if (!contains(&unit_interval, a) || !contains(&unit_interval, b))
		return (false);
	rec->uv[0] = a;
	rec->uv[1] = b;
	return (true);
}
