/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder_hit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 18:11:42 by lbrusa            #+#    #+#             */
/*   Updated: 2024/10/09 11:13:36 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "color.h"
#include "cylinder.h"
#include "material.h"
#include "ray.h"
#include "utils.h"
#include "vec3.h"
#include <stdio.h>

bool	solve_discriminant(double *t0, double *t1, t_cyl_utils *utils)
{
	t_vec3	delta_p;
	t_vec3	cross_rd_cd;
	t_vec3	cross_dp_cd;
	double	discr;

	delta_p = vec3substr(utils->r->orig, utils->cyl->center);
	cross_rd_cd = cross(utils->r->dir, utils->cyl->axis);
	cross_dp_cd = cross(delta_p, utils->cyl->axis);
	utils->a = dot(cross_rd_cd, cross_rd_cd);
	utils->b = 2 * dot(cross_rd_cd, cross_dp_cd);
	utils->c = dot(cross_dp_cd, cross_dp_cd) - pow(utils->cyl->radius, 2);
	discr = utils->b * utils->b - 4 * utils->a * utils->c;
	if (discr < 0)
		return (false);
	*t0 = (-utils->b - sqrt(discr)) / (2 * utils->a);
	*t1 = (-utils->b + sqrt(discr)) / (2 * utils->a);
	return (true);
}

bool	cylinder_intersection_check(double t, t_interval *ray_t,
		t_cyl_utils *utils)
{
	t_vec3	point;
	t_vec3	delta_point;
	double	height;

	if (surrounds(ray_t, t))
	{
		point = point_at(utils->r, t);
		delta_point = vec3substr(point, utils->cyl->center);
		height = dot(delta_point, utils->cyl->axis);
		if (utils->cyl->min < height && height < utils->cyl->max)
		{
			if (utils->closest_t < 0 || t < utils->closest_t)
			{
				utils->closest_t = t;
				utils->closest_point = point;
				utils->normal = unit_vector(vec3substr(point,
							vec3add(utils->cyl->center,
								vec3multscalar(utils->cyl->axis, height))));
				return (true);
			}
		}
	}
	return (false);
}

/**
 * @brief: check if the ray hits the uncapped cylinder
 *
 * The cylinder is an infinite object so we need to check if the intersection
 * is within the height of the cylinder
 */
bool	hit_cylinder(const void *self, const t_ray *r, t_interval ray_t,
		t_hit_record *rec)
{
	t_cyl_utils	utils;
	double		t0;
	double		t1;
	bool		hit;

	utils.cyl = (t_cylinder *)self;
	utils.r = r;
	if (!solve_discriminant(&t0, &t1, &utils))
		return (false);
	utils.closest_t = -1;
	utils.normal = vec3(0, 0, 0);
	utils.closest_point = point3(0, 0, 0);
	hit = cylinder_intersection_check(t0, &ray_t, &utils);
	hit = cylinder_intersection_check(t1, &ray_t, &utils) || hit;
	if (utils.closest_t >= 0 && hit)
	{
		rec->t = utils.closest_t;
		rec->p = utils.closest_point;
		rec->normal = utils.normal;
		set_face_normal(rec, r, rec->normal);
		rec->mat = utils.cyl->mat;
		get_cylinder_uncappedv(rec->normal, rec->uv);
		return (true);
	}
	return (false);
}

/**
 * @brief: check if the ray hits the cylinder capped object
 *
 * This is essentially creating a hittable list with the cylinder
 * and the two disks and using the hit function for the hittable
 * list on them
 */
bool	hit_cylinder_capped(const void *self, const t_ray *r,
		t_interval closest, t_hit_record *rec)
{
	const t_cylinder_capped	*c = (t_cylinder_capped *)self;
	t_hittablelist			cylinder_hittable_list;
	t_hittable				*list[3];

	list[0] = (t_hittable *)(&c->cylinder_uncapped);
	list[1] = (t_hittable *)(&c->top);
	list[2] = (t_hittable *)(&c->bottom);
	cylinder_hittable_list = hittablelist(list, 3);
	return (hit_objects(&cylinder_hittable_list, r, closest, rec));
}
