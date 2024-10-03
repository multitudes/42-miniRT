/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quad.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 09:13:07 by lbrusa            #+#    #+#             */
/*   Updated: 2024/10/03 11:59:19 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quad.h"
#include "hittable.h"
#include "utils.h"
#include "color.h"
#include <stdio.h>

/**
 * @brief Initialize a quad object with a given position q, and two vectors u and v.
 */
void	quad_rgb(t_quad *qd, t_point3 q, t_vec3 u, t_vec3 v, t_rgb rgbcolor)
{
	qd->base.hit = hit_quad;
	qd->base.pdf_value = quad_pdf_value;
	qd->base.random = quad_random;
	qd->q = q;
	qd->u = u;
	qd->v = v;
	qd->rgb = rgbcolor;
	qd->color = rgb_to_color(rgbcolor);
	t_vec3 n = cross(u, v);
	qd->normal = unit_vector(n);
	qd->d = dot(qd->normal, q);
	qd->w = vec3divscalar(n, dot(n, n));
	qd->area = length(n);
	solid_color_init(&(qd->texture), qd->color);
	lambertian_init_tex(&(qd->lambertian_mat), (t_texture*)&(qd->texture));
 	qd->mat = (t_material*)&(qd->lambertian_mat);
	qd->print = print_quad;
}


void quad_mat(t_quad *qd, t_point3 q, t_vec3 u, t_vec3 v, t_material *mat)
{
	qd->base.hit = hit_quad;
	qd->base.pdf_value = quad_pdf_value;
	qd->base.random = quad_random;
	qd->q = q;
	qd->u = u;
	qd->v = v;
	t_vec3 n = cross(u, v);
	qd->normal = unit_vector(n);
	qd->d = dot(qd->normal, q);
	qd->w = vec3divscalar(n, dot(n, n));
	qd->area = length(n);
 	qd->mat = mat;
	qd->print = print_quad;
}

/**
 * @brief: print the quad object
 *
 * @param: self: the quad object
 * the format will be
 * "quad %f,%f,%f    %f,%f,%f     %f,%f,%f      %d,%d,%d"
*/
void	print_quad(const void *self)
{
	const t_quad *qd = (t_quad *)self;
	printf("quad   %.f,%.f,%.f    %.f,%.f,%.f    %.f,%.f,%.f  %d,%d,%d\n",
	qd->q.x, qd->q.y, qd->q.z,
	qd->u.x, qd->u.y, qd->u.z,
	qd->v.x, qd->v.y, qd->v.z, 
	qd->rgb.r, qd->rgb.g, qd->rgb.b);
}

bool hit_quad(const void* self, const t_ray *r, t_interval ray_t,  t_hit_record *rec)
{
	const t_quad *qd = (t_quad *)self;
	double denom = dot(qd->normal, r->dir);
	if (fabs(denom) < 1e-8)
		return false;
	double t = (qd->d - dot(qd->normal, r->orig)) / denom;
	if (!contains(&ray_t, t))
		return false;
	t_point3 intersection = point_at(r, t);
	t_vec3 planar_hitpt_vector = vec3substr(intersection, qd->q);
	double alpha = dot(qd->w, cross(planar_hitpt_vector, qd->v));
	double beta = dot(qd->w, cross(qd->u, planar_hitpt_vector));
	if (!is_interior(alpha, beta, rec))
		return false;
	rec->t = t;
	rec->p = intersection;
	rec->mat = qd->mat;
	set_face_normal(rec, r, qd->normal);
	return true;
}

bool is_interior(double a, double b, t_hit_record *rec)
{
	t_interval unit_interval = interval(0, 1);
	if (!contains(&unit_interval, a) || !contains(&unit_interval, b) )
		return false;
	rec->uv[0] = a;
	rec->uv[1] = b;
	return true;
}

double quad_pdf_value(const void *self, const t_point3 *orig, const t_vec3 *dir)
{
	const t_quad *qd = (t_quad *)self;
	t_hit_record rec;
	const t_ray r = ray(*orig, *dir);
	if (!hit_quad(qd, &r, interval(0.001, 1e30), &rec))
		return 0;

	double distance_squared = len_sqrd(*dir) * rec.t * rec.t;
	double cosine = (fabs(dot(*dir, rec.normal))) / length(*dir);

	return distance_squared / (cosine * qd->area);
}

t_vec3 quad_random(const void *self, const t_point3 *orig)
{
	const t_quad *qd = (t_quad *)self;

	t_vec3 p = vec3add(qd->q, vec3add(vec3multscalar(qd->u, random_double(0, 1)), vec3multscalar(qd->v, random_double(0, 1))));
	return vec3substr(p, *orig);
}
