/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 10:52:10 by lbrusa            #+#    #+#             */
/*   Updated: 2024/09/15 18:39:28 by lbrusa           ###   ########.fr       */
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
#include "pdf.h"
#include "texture.h"

/*
 * @brief: initializer for a sphere
 * @param: center: the center of the sphere
 * @param: diameter: the diameter of the sphere
 * @param: rgbcolor: the color of the sphere
 * @return: a t_sphere struct
 *
 * This is also contains the initialization of the lambertian material
 * as hardcoded. Mainly so i can use it as default in the rt files
 */
t_sphere sphere(t_point3 center, double diameter, t_rgb rgbcolor)
{
	t_sphere s;
	s.base.hit = hit_sphere;
	s.base.pdf_value = obj_sphere_pdf_value;
	s.base.random = obj_sphere_random;
	s.center = center;
	s.radius = fmax(0, diameter / 2);
	s.rgb = rgbcolor;
	s.color = rgb_to_color(rgbcolor);
	// i use the color to create a texture
	solid_color_init(&(s.texture), s.color);
	// i init the lambertian material with the texture
	lambertian_init_tex(&(s.lambertian_mat), (t_texture*)&(s.texture));
	// i assign the material to the sphere as a pointer
	// the pointer will contain the scatter function for the material
	// which will be passed to the t_record struct when hit
 	s.mat = (t_material*)&(s.lambertian_mat); 
	s.print = print_sphere;
	return s;
}

/**
 * @brief: initializer for a sphere with a material
 * 
 * @param: center: the center of the sphere
 * @param: diameter: the diameter of the sphere
 * @param: rgbcolor: the color of the sphere
 * @param: mat: the material of the sphere
 * @return: a t_sphere struct
 * 
 * This initializer also allows to assign a material to the sphere
 * Supported are checkers and metal materials. Textures like solid color 
 * and images are also supported
 */
t_sphere sphere_mat(t_point3 center, double diameter, t_rgb rgbcolor, t_material *mat)
{
	(void)rgbcolor;
	t_sphere s;
	s.base.hit = hit_sphere;
	s.base.pdf_value = obj_sphere_pdf_value;
	s.base.random = obj_sphere_random;
	s.center = center;
	s.radius = fmax(0, diameter / 2);
	s.rgb = rgb(0,0,0);
	s.color = color(0,0,0);
 	s.mat = mat; 
	s.print = print_sphere;
	return s;
}

/** 
 * @brief prints the sphere information for the rt file
 *
 * like: sp 	0.0,0.020.6 	12.6	10,0,255
 * where the first 3 values are the center of the sphere
 * the 4th value is the diameter of the sphere
 * the last 3 values are the rgb color of the sphere
 */
void		print_sphere(const void *self)
{
	const t_sphere *s = (const t_sphere *)self;
	printf("sp\t%.f,%.f,%.f\t\t%.f\t\t%d,%d,%d\n", 
	s->center.x, s->center.y, s->center.z, s->radius * 2,
	s->rgb.r, s->rgb.g, s->rgb.b);
}

/**
 * @brief returns true if the ray hits the sphere
 * 
 * @param: self: the sphere object
 * @param: r: the ray
 * @param: ray_t: the interval of the ray
 * @param: rec: the hit record
 * @return: true if the ray hits the sphere
 * 
 * 
 * 
*/
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

/**
 * sphere_pdf_value - Computes the PDF value for a uniform sphere.
 * @self: Pointer to the object
 * @direction: Pointer to the t_vec3 direction vector
 *
 * This function returns the probability density function (PDF) value for a
 * uniformly distributed direction over the surface of a sphere. Since the
 * distribution is uniform, the PDF value is constant and equal to the inverse
 * of the surface area of the sphere, which is 1 / (4 * PI).
 *
 * Return: A double representing the PDF value for a uniform sphere.
 */
double obj_sphere_pdf_value(const void *self, const t_point3 *orig, const t_vec3 *dir)
{
	const t_sphere *s = (t_sphere *)self;

	t_hit_record rec;
	
	const t_ray r = ray(*orig, *dir);
	if (!hit_sphere(s, &r, interval(0.001, INFINITY), &rec))
		return 0;

    // Calculate distance squared from origin to sphere center
    double distance_squared = length_squared(vec3substr(s->center, *orig));

    // Calculate cosine of maximum theta (angle between ray and normal)
    double cos_theta_max = sqrt(1.0 - (s->radius * s->radius / distance_squared));
    double solid_angle = 2.0 * PI * (1.0 - cos_theta_max);

    // Return PDF (reciprocal of solid angle)
    return 1.0 / solid_angle;
	
}


t_vec3 obj_sphere_random(const void *self, const t_point3 *orig) 
{
    const t_sphere *s = (t_sphere *)self;

    // Calculate the direction vector from the origin to the sphere's center
    t_vec3 direction = vec3substr(s->center, *orig);

    // Calculate the squared distance from the origin to the sphere's center
    double distance_squared = length_squared(direction);

    // Build an orthonormal basis (ONB) from the direction vector
    t_onb uvw;
    onb_build_from_w(&uvw, &direction);

    t_vec3 random_point = random_to_sphere(s->radius, distance_squared);

    // Transform the random point using the ONB
    t_vec3 transformed_point = onb_transform(&uvw, random_point);
    return transformed_point;
}

// Function to generate a random direction within the sphere's volume
t_vec3 random_to_sphere(double radius, double distance_squared) 
{
    double r1 = random_d(); 
    double r2 = random_d();

    // Calculate z-coordinate based on uniform distribution within the unit sphere
    double z = 1.0 + r2 * (sqrt(1.0 - radius * radius / distance_squared) - 1.0);

    // Calculate phi (azimuthal angle) and x, y coordinates using spherical coordinates
    double phi = 2.0 * PI * r1;
    double x = cos(phi) * sqrt(1.0 - z * z);
    double y = sin(phi) * sqrt(1.0 - z * z);

    return unit_vector(vec3(x, y, z));  
}

t_rgb color_to_rgb(t_color color)
{
	t_rgb rgb;
	rgb.r = (uint8_t)(255.999 * color.x);
	rgb.g = (uint8_t)(255.999 * color.y);
	rgb.b = (uint8_t)(255.999 * color.z);
	return rgb;
}