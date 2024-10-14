/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   disk.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 09:13:07 by lbrusa            #+#    #+#             */
/*   Updated: 2024/10/14 11:05:12 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "disk.h"

/**
 * @brief Initialize a disk object with a given position, normal, and radius.
 *
 * @param disk disk object
 *
 * @param center center of the disk
 * @param normal normal vector of the disk
 * @param diam diameter of the disk
 * @param rgbcolor color of the disk
 */
void	disk(t_disk *d, t_init_params params)
{
	d->base.hit = hit_disk;
	d->base.pdf_value = disk_pdf_value;
	d->base.random = disk_random;
	d->center = params.center;
	d->radius = params.diam / 2;
	d->normal = unit_vector(params.normal);
	d->d = dot(d->normal, params.center);
	if (fabs(d->normal.x) > fabs(d->normal.y))
		d->u = vec3(-d->normal.z, 0, d->normal.x);
	else
		d->u = vec3(0, d->normal.z, -d->normal.y);
	d->u = unit_vector(d->u);
	d->v = unit_vector(cross(d->normal, d->u));
	d->u = vec3multscalar(d->u, params.diam / 2);
	d->v = vec3multscalar(d->v, params.diam / 2);
	d->w = vec3divscalar(cross(d->u, d->v), dot(cross(d->u, d->v), \
				cross(d->u, d->v)));
	d->rgb = params.rgbcolor;
	d->color = rgb_to_color(params.rgbcolor);
	solid_color_init(&(d->texture), d->color);
	lambertian_init_tex(&(d->lambertian_mat), (t_texture *)&(d->texture));
	d->mat = (t_material *)&(d->lambertian_mat);
}

/**
 * @brief Initialize a disk object with a given position, normal, and radius.
 *
 * @param disk disk object
 * @param center center of the disk
 * @param u first vector of the disk
 * @param v second vector of the disk
 * @param mat material of the disk
 */
void	disk_mat(t_disk *d, t_init_params params)
{
	d->base.hit = hit_disk;
	d->base.pdf_value = disk_pdf_value;
	d->base.random = disk_random;
	d->center = params.center;
	d->radius = params.diam / 2;
	d->normal = unit_vector(params.normal);
	d->d = dot(d->normal, params.center);
	if (fabs(d->normal.x) > fabs(d->normal.y))
		d->u = unit_vector(vec3(-d->normal.z, 0, d->normal.x));
	else
		d->u = unit_vector(vec3(0, d->normal.z, -d->normal.y));
	d->v = vec3multscalar(unit_vector(cross(d->normal, d->u)), params.diam / 2);
	d->u = vec3multscalar(d->u, params.diam / 2);
	d->w = vec3divscalar(cross(d->u, d->v), dot(cross(d->u, d->v), \
				cross(d->u, d->v)));
	d->rgb = rgb(0, 0, 0);
	d->color = color(0, 0, 0);
	d->mat = params.mat;
}

/**
 * @brief Check if a ray hits a disk
 *
 * @param self the disk object
 * @return true if the ray hits the disk
 *
 * We have no hit if ray is parallel to the disk
 * We return false if the hit point parameter t is outside the ray interval.
 *
 */
bool	hit_disk(const void *self, const t_ray *r, t_interval ray_t,
		t_hit_record *rec)
{
	const t_disk	*disk = (t_disk *)self;
	double			denom;
	double			t;
	t_point3		intersection;
	t_vec3			planar_hitpt_vector;

	denom = dot(disk->normal, r->dir);
	if (fabs(denom) < 1e-8)
		return (false);
	t = (disk->d - dot(disk->normal, r->orig)) / denom;
	if (!contains(&ray_t, t))
		return (false);
	intersection = point_at(r, t);
	planar_hitpt_vector = vec3substr(intersection, disk->center);
	if (len_sqrd(planar_hitpt_vector) > disk->radius * disk->radius)
		return (false);
	rec->t = t;
	rec->p = intersection;
	rec->mat = disk->mat;
	set_face_normal(rec, r, disk->normal);
	return (true);
}
