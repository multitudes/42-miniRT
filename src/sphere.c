/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 10:52:10 by lbrusa            #+#    #+#             */
/*   Updated: 2024/10/13 14:09:23 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "color.h"
#include "hittable.h"
#include "interval.h"
#include "material.h"
#include "ray.h"
#include "sphere.h"
#include "texture.h"
#include "utils.h"
#include "vec3.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include "libft.h"


/*
 * @brief: initializer for a sphere
 * @param: center: the center of the sphere
 * @param: diameter: the diameter of the sphere
 * @param: rgbcolor: the color of the sphere
 * @return: a t_sphere struct
 *
 * This is also contains the initialization of the lambertian material
 * as hardcoded. Mainly so i can use it as default in the rt files.
 * In this way we simplify the parser and the rt files. Also since we
 * keep everything on the stack we need to initialize the material
 * by using the address of the texture. This will be a property of the
 * sphere object.
 */
void	sphere(t_sphere *s, t_init_params params)
{
	s->base.hit = hit_sphere;
	s->base.pdf_value = obj_sphere_pdf_value;
	s->base.random = obj_sphere_random;
	s->center = params.center;
	s->radius = fmax(0, params.diam / 2);
	s->rgb = params.rgbcolor;
	s->color = rgb_to_color(params.rgbcolor);
	solid_color_init(&(s->texture), s->color);
	lambertian_init_tex(&(s->lambertian_mat), (t_texture *)&(s->texture));
	s->mat = (t_material *)&(s->lambertian_mat);
	s->print = print_sphere;
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
void	sphere_mat(t_sphere *s, t_init_params params)
{
	s->base.hit = hit_sphere;
	s->base.pdf_value = obj_sphere_pdf_value;
	s->base.random = obj_sphere_random;
	s->center = params.center;
	s->radius = fmax(0, params.diam / 2);
	s->rgb = rgb(0, 0, 0);
	s->color = color(0, 0, 0);
	s->mat = params.mat;
	s->print = print_sphere;
}

/**
 * @brief prints the sphere information for the rt file
 *
 * like: sp 	0.0,0.020.6 	12.6	10,0,255
 * where the first 3 values are the center of the sphere
 * the 4th value is the diameter of the sphere
 * the last 3 values are the rgb color of the sphere
 */
void	print_sphere(const void *self)
{
	const t_sphere	*s = (const t_sphere *)self;

	ft_printf("sp\t%.f,%.f,%.f\t\t%.f\t\t%d,%d,%d\n", s->center.x, s->center.y,
		s->center.z, s->radius * 2, s->rgb.r, s->rgb.g, s->rgb.b);
}

bool	calculate_root(const t_sphere *s, const t_ray *r, t_interval ray_t,
		double *root)
{
	double	a;
	double	h;
	double	c;
	double	discriminant;
	double	squared;

	a = len_sqrd(r->dir);
	h = dot(r->dir, vec3substr(s->center, r->orig));
	c = len_sqrd(vec3substr(s->center, r->orig)) - (s->radius * s->radius);
	discriminant = h * h - a * c;
	if (discriminant < 0)
		return (false);
	squared = sqrt(discriminant);
	*root = (h - squared) / a;
	if (!surrounds(&ray_t, *root))
	{
		*root = (h + squared) / a;
		if (!surrounds(&ray_t, *root))
			return (false);
	}
	return (true);
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
 */
bool	hit_sphere(const void *self, const t_ray *r, t_interval ray_t,
		t_hit_record *rec)
{
	const t_sphere	*s = (t_sphere *)self;
	double			root;
	t_vec3			outward_normal;

	root = 0;
	if (!calculate_root(s, r, ray_t, &root))
		return (false);
	rec->t = root;
	rec->p = point_at(r, root);
	outward_normal = vec3divscalar(vec3substr(rec->p, s->center), s->radius);
	set_face_normal(rec, r, outward_normal);
	get_sphere_uv(rec->normal, rec->uv);
	rec->mat = s->mat;
	return (true);
}
