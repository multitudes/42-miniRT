/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   disk.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 09:13:07 by lbrusa            #+#    #+#             */
/*   Updated: 2024/10/03 15:06:30 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "debug.h"
#include "disk.h"
#include "hittable.h"
#include "minirt.h"
#include "utils.h"
#include <stdio.h>

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
void	disk(t_disk *d, t_point3 center, t_vec3 normal, double diam,
		t_rgb rgbcolor)
{
	t_vec3	u;
	t_vec3	v;

	d->base.hit = hit_disk;
	d->base.pdf_value = disk_pdf_value;
	d->base.random = disk_random;
	d->center = center;
	d->radius = diam / 2;
	d->normal = unit_vector(normal);
	d->d = dot(d->normal, center);
	if (fabs(d->normal.x) > fabs(d->normal.y))
		u = vec3(-d->normal.z, 0, d->normal.x);
	else
		u = vec3(0, d->normal.z, -d->normal.y);
	u = unit_vector(u);
	v = unit_vector(cross(d->normal, u));
	u = vec3multscalar(u, diam / 2);
	v = vec3multscalar(v, diam / 2);
	d->u = u;
	d->v = v;
	d->w = vec3divscalar(cross(u, v), dot(cross(u, v), cross(u, v)));
	d->rgb = rgbcolor;
	d->color = rgb_to_color(rgbcolor);
	solid_color_init(&(d->texture), d->color);
	lambertian_init_tex(&(d->lambertian_mat), (t_texture *)&(d->texture));
	d->mat = (t_material *)&(d->lambertian_mat);
	d->print = print_disk;
}

// will be the new int from params
void	disk_o(t_disk *d, t_init_params obj)
{
	t_vec3	u;
	t_vec3	v;

	d->base.hit = hit_disk;
	d->base.pdf_value = disk_pdf_value;
	d->base.random = disk_random;
	d->center = obj.center;
	d->radius = obj.diam / 2;
	d->normal = unit_vector(obj.normal);
	d->d = dot(d->normal, obj.center);
	if (fabs(d->normal.x) > fabs(d->normal.y))
		u = unit_vector(vec3(-d->normal.z, 0, d->normal.x));
	else
		u = unit_vector(vec3(0, d->normal.z, -d->normal.y));
	v = vec3multscalar(unit_vector(cross(d->normal, u)), obj.diam / 2);
	u = vec3multscalar(u, obj.diam / 2);
	d->u = u;
	d->v = v;
	d->w = vec3divscalar(cross(u, v), dot(cross(u, v), cross(u, v)));
	d->rgb = obj.rgbcolor;
	d->color = rgb_to_color(obj.rgbcolor);
	solid_color_init(&(d->texture), d->color);
	lambertian_init_tex(&(d->lambertian_mat), (t_texture *)&(d->texture));
	d->mat = (t_material *)&(d->lambertian_mat);
	d->print = print_disk;
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
void	disk_mat(t_disk *d, t_point3 center, t_vec3 normal, double diam,
		t_material *mat)
{
	t_vec3	u;
	t_vec3	v;

	d->base.hit = hit_disk;
	d->base.pdf_value = disk_pdf_value;
	d->base.random = disk_random;
	d->center = center;
	d->radius = diam / 2;
	d->normal = unit_vector(normal);
	d->d = dot(d->normal, center);
	if (fabs(d->normal.x) > fabs(d->normal.y))
		u = unit_vector(vec3(-d->normal.z, 0, d->normal.x));
	else
		u = unit_vector(vec3(0, d->normal.z, -d->normal.y));
	v = vec3multscalar(unit_vector(cross(d->normal, u)), diam / 2);
	u = vec3multscalar(u, diam / 2);
	d->u = u;
	d->v = v;
	d->w = vec3divscalar(cross(u, v), dot(cross(u, v), cross(u, v)));
	d->rgb = rgb(0, 0, 0);
	d->color = color(0, 0, 0);
	d->mat = mat;
	d->print = print_disk;
}

void	disk_mat_o(t_disk *d, t_init_params obj)
{
	t_vec3	u;
	t_vec3	v;

	d->base.hit = hit_disk;
	d->base.pdf_value = disk_pdf_value;
	d->base.random = disk_random;
	d->center = obj.center;
	d->radius = obj.diam / 2;
	d->normal = unit_vector(obj.normal);
	d->d = dot(d->normal, obj.center);
	if (fabs(d->normal.x) > fabs(d->normal.y))
		u = unit_vector(vec3(-d->normal.z, 0, d->normal.x));
	else
		u = unit_vector(vec3(0, d->normal.z, -d->normal.y));
	v = vec3multscalar(unit_vector(cross(d->normal, u)), obj.diam / 2);
	u = vec3multscalar(u, obj.diam / 2);
	d->u = u;
	d->v = v;
	d->w = vec3divscalar(cross(u, v), dot(cross(u, v), cross(u, v)));
	d->rgb = rgb(0, 0, 0);
	d->color = color(0, 0, 0);
	d->mat = obj.mat;
	d->print = print_disk;
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
