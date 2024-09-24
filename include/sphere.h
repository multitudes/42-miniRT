/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 10:52:56 by lbrusa            #+#    #+#             */
/*   Updated: 2024/09/16 17:37:11 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SPHERE_H
# define SPHERE_H

#include "texture.h"
#include "vec3.h"
#include "ray.h"
#include <stdbool.h>
#include "hittable.h"
#include "color.h"
#include "material.h"

typedef struct		s_sphere
{
	t_hittable  	base;
	t_point3		center;
	double			radius;
	t_color			color;
	t_rgb			rgb;
	t_material		*mat;
	union
	{
		t_lambertian 	lambertian_mat;
		t_metal				metal;
	};
	union
	{
		t_checker_texture	checker;
		t_solid_color 		texture;
		t_img_texture		img_texture;
	};
	void (*print)(const void *self);
}					t_sphere;

/*
 * a sort of initializer for a sphere
 */
void	sphere(t_sphere *s, t_point3 center, double diameter, t_rgb color);
void	sphere_mat(t_sphere *s, t_point3 center, double diameter, t_material *mat);

void		print_sphere(const void *self);
void		print_sphere_mat(const void *self);
/* if the ray hits the sphere, return the t value */
bool		hit_sphere(const void* self, const t_ray *r, t_interval closest, t_hit_record *rec);
void		get_sphere_uv(t_vec3 normal, double* u, double* v);
t_rgb 		color_to_rgb(t_color color);

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
double obj_sphere_pdf_value(const void *self, const t_point3 *orig, const t_vec3 *dir);
t_vec3 obj_sphere_random(const void *self, const t_point3 *orig);
// Function to generate a random direction within the sphere's volume
t_vec3 random_to_sphere(double radius, double distance_squared);

#endif
