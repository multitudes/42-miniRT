/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 14:59:39 by lbrusa            #+#    #+#             */
/*   Updated: 2024/10/14 11:05:50 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "plane.h"
#include <stdio.h>

/**
 * @brief plane - Initialize a plane object
 *
 * @param pl the plane object (already on the stack)
 * @param point a point in the plane
 * @param normal the normal vector of the plane
 * @param rgbcol the color of the plane
 *
 * The plane is defined by a point and a normal vector to the plane
 * The normal vector is normalized. I use PLANE_MAX to limit the plane to
 * avoid wasting resources since the ray will take a long time to hit the plane
 */
void	plane(t_plane *pl, t_init_params params)
{
	pl->base.hit = hit_plane;
	pl->base.pdf_value = plane_pdf_value;
	pl->base.random = plane_random;
	pl->q = params.center;
	pl->normal = unit_vector(params.normal);
	pl->d = -dot(pl->normal, params.center);
	pl->u = vec3(PLANE_MAX, PLANE_MAX, PLANE_MAX);
	pl->v = vec3(PLANE_MAX, PLANE_MAX, PLANE_MAX);
	pl->rgb = params.rgbcolor;
	pl->color = rgb_to_color(params.rgbcolor);
	solid_color_init(&(pl->solid), pl->color);
	lambertian_init_tex(&(pl->lambertian_mat), (t_texture *)&(pl->solid));
	pl->mat = (t_material *)&(pl->lambertian_mat);
}

void	plane_mat(t_plane *pl, t_init_params params)
{
	pl->base.hit = hit_plane;
	pl->base.pdf_value = plane_pdf_value;
	pl->base.random = plane_random;
	pl->q = params.center;
	pl->normal = unit_vector(params.normal);
	pl->d = -dot(pl->normal, params.center);
	pl->u = vec3(PLANE_MAX, PLANE_MAX, PLANE_MAX);
	pl->v = vec3(PLANE_MAX, PLANE_MAX, PLANE_MAX);
	pl->mat = params.mat;
	pl->rgb = rgb(0, 0, 0);
	pl->color = color(0, 0, 0);
}

/**
 * @brief hit_plane - Check if a ray hits a plane
 *
 * @param self the plane object
 * @param r the ray
 * @param ray_t the interval of the ray
 * @param rec the hit record
 * @return true if the ray hits the plane
 *
 * 	no hit if ray is parallel to the plane
 */
bool	hit_plane(const void *self, const t_ray *r, t_interval ray_t,
		t_hit_record *rec)
{
	const t_plane	*pl = (t_plane *)self;
	double			denom;
	double			t;

	denom = dot(pl->normal, r->dir);
	if (fabs(denom) < 1e-8)
		return (false);
	t = (-pl->d - dot(pl->normal, r->orig)) / denom;
	if (!contains(&ray_t, t))
		return (false);
	rec->t = t;
	rec->p = point_at(r, t);
	rec->mat = pl->mat;
	set_face_normal(rec, r, pl->normal);
	return (true);
}
