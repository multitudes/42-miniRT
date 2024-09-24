/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   disk.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 09:13:07 by lbrusa            #+#    #+#             */
/*   Updated: 2024/09/24 11:40:30 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "disk.h"
#include "hittable.h"
#include <stdio.h>
#include "utils.h"
#include "debug.h"


void disk(t_disk *d, t_point3 center, t_vec3 normal, double diam, t_rgb rgbcolor)
{
    d->base.hit = hit_disk;
    d->base.pdf_value = disk_pdf_value;
    d->base.random = disk_random;
    d->center = center;
	d->radius = diam /2;
    d->normal = unit_vector(normal);
    d->d = dot(d->normal, center);

    // Create two orthogonal vectors u and v in the plane of the disk
    t_vec3 u, v;
    if (fabs(d->normal.x) > fabs(d->normal.y)) {
        u = vec3(-d->normal.z, 0, d->normal.x);
    } else {
        u = vec3(0, d->normal.z, -d->normal.y);
    }
    u = unit_vector(u);
    v = unit_vector(cross(d->normal, u));

	u = vec3multscalar(u, diam / 2);
	v = vec3multscalar(v, diam / 2);
    d->u = u;
    d->v = v;
    d->w = vec3divscalar(cross(u, v), dot(cross(u, v), cross(u, v)));
    d->rgb = rgbcolor;
    d->color = rgb_to_color(rgbcolor);

    // Initialize texture and material as I did for the quad
    solid_color_init(&(d->texture), d->color);
    lambertian_init_tex(&(d->lambertian_mat), (t_texture*)&(d->texture));
    d->mat = (t_material*)&(d->lambertian_mat);
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
void disk_mat(t_disk *d, t_point3 center, t_vec3 normal, double diam, t_material *mat)
{
 	d->base.hit = hit_disk;
    d->base.pdf_value = disk_pdf_value;
    d->base.random = disk_random;
    d->center = center;
	d->radius = diam / 2;
    d->normal = unit_vector(normal);
    d->d = dot(d->normal, center);

    // Create two orthogonal vectors u and v in the plane of the disk
    t_vec3 u, v;
    if (fabs(d->normal.x) > fabs(d->normal.y)) {
        u = vec3(-d->normal.z, 0, d->normal.x);
    } else {
        u = vec3(0, d->normal.z, -d->normal.y);
    }
    u = unit_vector(u);
    v = unit_vector(cross(d->normal, u));

	u = vec3multscalar(u, diam / 2);
	v = vec3multscalar(v, diam / 2);
    d->u = u;
    d->v = v;
    d->w = vec3divscalar(cross(u, v), dot(cross(u, v), cross(u, v)));
	d->rgb = rgb(0, 0, 0);	
    d->color = color(0, 0, 0);
	d->mat = mat;
	d->print = print_disk;
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
	d->center.x, d->center.y, d->center.z, 
	d->u.x, d->u.y, d->u.z, 
	d->v.x, d->v.y, d->v.z, 
	d->rgb.r, d->rgb.g, d->rgb.b);
}


bool hit_disk(const void* self, const t_ray *r, t_interval ray_t,  t_hit_record *rec)
{
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
	t_vec3 planar_hitpt_vector = vec3substr(intersection, disk->center);
	    // Determine the hit point lies within the planar shape using its plane coordinates.
    double distance_squared = length_squared(planar_hitpt_vector);
	
    // Check if the intersection point is within the disk's radius
    if (distance_squared > disk->radius * disk->radius)
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
    if (!hit_disk(d, &r, interval(0.001, 1e30), &rec))
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
    t_vec3 point_on_disk = vec3add(d->center, vec3add(vec3multscalar(d->u, r * cos(theta)), vec3multscalar(d->v, r * sin(theta))));
    t_vec3 direction = vec3substr(point_on_disk, *orig);
    return unit_vector(direction);
}
