/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 10:52:10 by lbrusa            #+#    #+#             */
/*   Updated: 2024/09/06 20:13:07 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <math.h>
#include "sphere.h"
#include "hittable.h"
#include "vec3.h"
#include "ray.h"
#include "interval.h"
#include "color.h"
#include <stdio.h>
#include "material.h"
#include "debug.h"

/*
 * a sort of initializer for a sphere
 */
t_sphere sphere(t_point3 center, double diameter, t_rgb rgbcolor)
{
	t_sphere s;
	s.base.hit = hit_sphere;
	s.center = center;
	s.radius = diameter / 2;
	s.print = print_sphere;
	s.rgb = rgbcolor;
	s.color = rgb_to_color(rgbcolor);
	debug("color: %f %f %f\n", s.color.r, s.color.g, s.color.b);
	// solid_color_init(&(s.texture), s.color);
	lambertian_init(&(s.lambertian_mat), s.color);
	s.mat = (t_material*)&(s.lambertian_mat); 
	return s;
}

/** 
 * @brief print the sphere information
 * in the rt file format
 * like sp 	0.0,0.020.6 	12.6	10,0,255
 */
void		print_sphere(const void *self)
{
	const t_sphere *s = (const t_sphere *)self;
	printf("sp\t%.f,%.f,%.f\t\t%.f\t\t%d,%d,%d\n", 
	s->center.x, s->center.y, s->center.z, s->radius * 2,
	s->rgb.r, s->rgb.g, s->rgb.b);
}

bool hit_sphere(const void* self, const t_ray* r, t_interval ray_t, t_hit_record* rec)
{
	const t_sphere* s = (t_sphere*)self;

	t_vec3 oc = vec3substr(s->center, r->orig);
	
	double a = length_squared(r->dir);
	double h = dot(r->dir, oc);
	double c = length_squared(oc) - (s->radius * s->radius);
	double discriminant = h * h - a * c;

	if (discriminant < 0)
		return false;
	 
	double squared = sqrt(discriminant);

	double root = (h - squared ) / a;

	if (!surrounds(&ray_t, root))
	{
		root = (h + squared ) / a;
		if (!surrounds(&ray_t, root))
			return false;
	}

	rec->t = root;
	rec->p = point_at(r, root);
	rec->mat = s->mat;
	t_vec3 outward_normal = vec3divscalar(vec3substr(rec->p, s->center), s->radius);
	set_face_normal(rec, r, outward_normal); 
	
	return true;

	
}


// /*
//  * The formula for a sphere is derived from the equation of a sphere
//  * (p - c) * (p - c) = r * r
//  * The func takes a first param of type void* to be able to be used in 
//  * the hittable list (sort of polymorphic behaviour)
//  * in the body oc is the vector from origin of the ray 
//  * to the center of the sphere
//  * At first the formula was derived from the quadratic formula
//  * double b = -2.0 * dot(&(r->dir), &oc);
//  * double c = dot(&oc, &oc) - s->radius * s->radius;
//  * but this has been refactored using double h 
//  */
// bool hit_sphere(const void *self, const t_ray *r, double ray_tmin, double ray_tmax, t_hit_record *rec) 
// {
// 	const t_sphere *s = (t_sphere *)self;
//     t_vec3 oc = vec3substr(&(s->center), &(r->orig));
//     double a = length3_squared(&r->dir); 
//     double h = dot(&(r->dir), &oc);
// 	double c = length3_squared(&oc) - s->radius * s->radius;
//     double discriminant = h*h - a*c;

// 	if (discriminant < 0)
// 		return (false);
// 	double sqrtd = sqrt(discriminant);
// 	double root = (h - sqrtd) / a;
// 	if (root <= ray_tmin || ray_tmax <= root) {
// 	root = (h + sqrtd) / a;
// 	if (root <= ray_tmin || ray_tmax <= root)
// 		return false;
//     }
// 	rec->t = root;
// 	rec->p = point_at(r, rec->t);
// 	t_vec3 inters_minus_center = vec3substr(&rec->p, &(s->center));
// 	rec->normal = vec3divscalar(&inters_minus_center, s->radius);
// 	set_face_normal(rec, r, &rec->normal);

// 	return (true);
// }


void set_face_normal(t_hit_record *rec, const t_ray *r, const t_vec3 outward_normal)
{
	rec->front_face = dot(r->dir, outward_normal) < 0;
	rec->normal = rec->front_face ? outward_normal : vec3negate(outward_normal);
}