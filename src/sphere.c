/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 10:52:10 by lbrusa            #+#    #+#             */
/*   Updated: 2024/09/07 21:36:04 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include "sphere.h"
#include "hittable.h"
#include "vec3.h"
#include "ray.h"
#include "interval.h"
#include "color.h"
#include "material.h"
#include "debug.h"
#include "utils.h"

/*
 * @brief: initializer for a sphere
 * @param: center: the center of the sphere
 * @param: diameter: the diameter of the sphere
 * @param: rgbcolor: the color of the sphere
 * @return: a t_sphere struct
 *
 * This is also contains the initialization of the lambertian material
 * as hardcoded. Using textures allows to reuse this structure for other
 * textures like checkers
 */
void	sphere(t_sphere *s, t_point3 center, double diameter, t_rgb rgbcolor)
{
	s->base.hit = hit_sphere;
	s->center = center;
	s->radius = fmax(0, diameter / 2);
	s->rgb = rgbcolor;
	s->color = rgb_to_color(rgbcolor);
	// i use the color to create a texture
	solid_color_init(&(s->texture), s->color);
	// i init the lambertian material with the texture
	lambertian_init_tex(&(s->lambertian_mat), (t_texture*)&(s->texture));
	// i assign the material to the sphere as a pointer
	// the pointer will contain the scatter function for the material
	// which will be passed to the t_record struct when hit
 	s->mat = (t_material*)&(s->lambertian_mat);
}

t_sphere sphere_mat(t_point3 center, double diameter, t_rgb rgbcolor, t_material *mat)
{
	t_sphere s;
	s.base.hit = hit_sphere;
	s.center = center;
	s.radius = fmax(0, diameter / 2);
	s.rgb = rgbcolor;
	s.color = rgb_to_color(rgbcolor);
 	s.mat = mat;
	debug("sphere_mat emit: %p", s.mat->emit);
	return s;
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
	t_vec3 outward_normal = vec3divscalar(vec3substr(rec->p, s->center), s->radius);
	set_face_normal(rec, r, outward_normal);
	get_sphere_uv(rec->normal, &rec->u, &rec->v);

	rec->mat = s->mat;
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


/*
 * get_sphere_uv
 *
 * p: point / the outward normal on the sphere
 * u: u
 * v: v
 *
 * returns: the uv coordinates of a sphere
 */
void	get_sphere_uv(t_vec3 normal, double* u, double* v)
{
    double theta;
    double phi;

	theta = acos(-normal.y);
	phi = atan2(-normal.z, normal.x) + PI;
    *u = phi / (2 * PI);
    *v = theta / PI;
}
