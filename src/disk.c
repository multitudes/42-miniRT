/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   disk.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 09:13:07 by lbrusa            #+#    #+#             */
/*   Updated: 2024/09/16 16:21:55 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "disk.h"
#include "hittable.h"
#include <stdio.h>
#include "utils.h"

void	disk(t_disk *d, t_point3 q, t_vec3 u, t_vec3 v, t_rgb rgbcolor)
{
	d->base.hit = hit_disk;
	d->base.pdf_value = disk_pdf_value;
	d->base.random = disk_random;
	d->q = q;
	d->u = u;
	d->v = v;
	t_vec3 n = cross(u, v);
    d->normal = unit_vector(n);
    d->d = dot(d->normal, q);
	d->w = vec3divscalar(n, dot(n, n));
	
	d->rgb = rgbcolor;
    d->color = rgb_to_color(rgbcolor);

    // Initialize texture and material as I did for the quad
    solid_color_init(&(d->texture), d->color);
    lambertian_init_tex(&(d->lambertian_mat), (t_texture*)&(d->texture));
    d->mat = (t_material*)&(d->lambertian_mat);
	d->print = print_disk;

}

void disk_mat(t_disk *disk, t_point3 q, t_vec3 u, t_vec3 v, t_material *mat)
{
	disk->base.hit = hit_disk;
	disk->q = q;
	disk->u = u;
	disk->v = v;
	disk->mat = mat;
	t_vec3 n = cross(u, v);
    disk->normal = unit_vector(n);
    disk->d = dot(disk->normal, q);
	disk->w = vec3divscalar(n, dot(n, n));
	// colors depend of material and will be calc in the scatter function
	disk->rgb = rgb(0, 0, 0);	
    disk->color = color(0, 0, 0);
	disk->mat = mat;
	disk->print = print_disk;
}

/**
 * @brief print the disk object
 * 
 * format is like this 
 * disk 0.0,0.0,-10.0 0.0,1.0,0.0 0,0,225
 */
void		print_disk(const void *self)
{
	const t_disk *d = (const t_disk *)self;
	printf("disk\t%.f,%.f,%.f\t\t%.f,%.f,%.f\t\t%.f,%.f,%.f\t\t\t%d,%d,%d\n", 
	d->q.x, d->q.y, d->q.z, 
	d->u.x, d->u.y, d->u.z, 
	d->v.x, d->v.y, d->v.z, 
	d->rgb.r, d->rgb.g, d->rgb.b);
}


bool hit_disk(const void* self, const t_ray *r, t_interval ray_t,  t_hit_record *rec)
{
	// printf("hit_quad ----------------------********\n");
	const t_disk *disk = (t_disk *)self;
	double denom = dot(disk->normal, r->dir);
	// no hit if ray is parallel to the quad
	if (fabs(denom) < 1e-8)
		return false;

	// Return false if the hit point parameter t is outside the ray interval.
	double t = (disk->d - dot(disk->normal, r->orig)) / denom;
	if (!contains(&ray_t, t))
		return false;
	
	// Determine the hit point lies within the planar shape using its plane coordinates.
	// t_point3	point_at(const t_ray *ray, double t)
	t_point3 intersection = point_at(r, t);
	t_vec3 planar_hitpt_vector = vec3substr(intersection, disk->q);
	double alpha = dot(disk->w, cross(planar_hitpt_vector, disk->v));
	double beta = dot(disk->w, cross(disk->u, planar_hitpt_vector));

	if (!is_interior_disk(alpha, beta, rec, disk->u, disk->v))
		return false;

	// Ray hits the 2D shape; set the rest of the hit record and return true.
	rec->t = t;
	rec->p = intersection;
	rec->mat = disk->mat;
	set_face_normal(rec, r, disk->normal);

	return true;
}

bool is_interior_disk(double a, double b, t_hit_record *rec, t_vec3 u, t_vec3 v) 
{
    // The center of the disk in plane coordinates is (0.5, 0.5)
    double u_squared = (a - 0.5) * (a - 0.5);
    double v_squared = (b - 0.5) * (b - 0.5);
    double uv = (a - 0.5) * (b - 0.5);

    // The radius of the disk is 0.5 in plane coordinates
    double radius_squared = 0.25;

    // The angle between the vectors u and v
    double cos_theta = dot(u, v) / (length(u) * length(v));

    // The distance from the point to the center of the disk in the plane coordinates of the parallelogram
    double distance_squared = u_squared + v_squared - 2 * uv * cos_theta;

    if (distance_squared > radius_squared)
        return false;

    rec->u = a;
    rec->v = b;
    return true;
}

double disk_pdf_value(const void *self, const t_point3 *orig, const t_vec3 *dir) 
{
    const t_disk *d = (t_disk *)self;
    t_hit_record rec;
    const t_ray r = ray(*orig, *dir);
    if (!hit_disk(d, &r, interval(0.001, INFINITY), &rec))
        return 0;

    // Calculate the distance squared from the origin to the hit point
    double distance_squared = length_squared(vec3substr(rec.p, *orig));

    // Calculate the cosine of the angle between the ray and the disk normal
    double cosine = fabs(dot(*dir, d->normal));

    // Calculate the area of the disk
    double radius = length(d->u);
    double area = PI * radius * radius;

    return distance_squared / (cosine * area);
}

t_vec3 disk_random(const void *self, const t_point3 *orig) 
{
    const t_disk *d = (t_disk *)self;

    // Generate random points within the unit circle
    double r = sqrt(random_double(0, 1));
    double theta = 2 * PI * random_double(0, 1);

    // Calculate the point on the disk corresponding to the random coordinates
    t_vec3 p = vec3add(d->q, vec3add(vec3multscalar(d->u, r * cos(theta)), vec3multscalar(d->v, r * sin(theta))));

    return vec3substr(p, *orig);
}