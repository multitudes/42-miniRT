/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone_hit.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 16:06:09 by ralgaran          #+#    #+#             */
/*   Updated: 2024/09/30 09:59:18 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cone.h"
#include "utils.h"
#include <math.h>
#include <stdbool.h>

static bool	cone_quadratic(const t_cone_uncap *cone, const t_ray *r, \
	double *t0, double *t1)
{
	double			a;
	double			b;
	double			c;
	double			disc;
	t_vec3			delta_p;

	delta_p = vec3substr(r->orig, cone->apex);
	a = dot(cross(r->dir, cone->axis), cross(r->dir, cone->axis)) - \
		dot(r->dir, r->dir) * pow(cone->radius / cone->height, 2);
	b = 2 * (dot(cross(r->dir, cone->axis), cross(delta_p, cone->axis)) - \
		dot(r->dir, delta_p) * pow(cone->radius / cone->height, 2));
	c = dot(cross(delta_p, cone->axis), cross(delta_p, cone->axis)) - \
		dot(delta_p, delta_p) * pow(cone->radius / cone->height, 2);
	disc = b * b - 4 * a * c;
	if (disc < 0)
		return (false);
	*t0 = (-b - sqrt(disc)) / (2 * a);
	*t1 = (-b + sqrt(disc)) / (2 * a);
	return (true);
}

static void	cone_intesection_check(double t, t_interval *ray_t, \
	t_cone_utils *utils)
{
	t_vec3	point;
	t_vec3	delta_point;
	double	height;
	t_vec3	v_proj;

	if (surrounds(ray_t, t))
	{
		point = point_at(utils->r, t);
		delta_point = vec3substr(point, utils->cone->apex);
		height = dot(delta_point, utils->cone->axis);
		if (0 <= height && height <= utils->cone->height)
		{
			if (utils->closest_t < 0 || t < utils->closest_t)
			{
				utils->closest_t = t;
				utils->closest_point = point;
				v_proj = vec3multscalar(utils->cone->axis, \
					dot(delta_point, utils->cone->axis));
				utils->normal = unit_vector(vec3substr(delta_point, v_proj));
			}
		}
	}
}

bool	hit_cone(const void *self, const t_ray *r, t_interval ray_t, \
	t_hit_record *rec)
{
	t_cone_utils	utils;
	double			t0;
	double			t1;

	utils.cone = (t_cone_uncap *)self;
	utils.r = r;
	if (cone_quadratic(utils.cone, utils.r, &t0, &t1) == false)
		return (false);
	utils.closest_t = -1;
	cone_intesection_check(t0, &ray_t, &utils);
	cone_intesection_check(t1, &ray_t, &utils);
	if (utils.closest_t >= 0)
	{
		rec->t = utils.closest_t;
		rec->p = utils.closest_point;
		rec->normal = utils.normal;
		set_face_normal(rec, r, rec->normal);
		rec->mat = utils.cone->mat;
		return (true);
	}
	return (false);
}
