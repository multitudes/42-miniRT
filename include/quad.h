/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quad.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 09:07:01 by lbrusa            #+#    #+#             */
/*   Updated: 2024/09/22 13:20:03 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef QUAD_H
# define QUAD_H

# include "vec3.h"
# include "interval.h"
# include "material.h"
# include "ray.h"
# include "hittable.h"

/*
 * A quad is a 4-pointed polygon in 3D space
 *
 * q: the center of the quad
 * u: the first vector of the quad
 * v: the second vector of the quad
 * mat: the material of the quad
 *
 * The miniRT quad is not in the mandatory part and 
 * could be also be defined by the center point q
 * and a normalized vector and a color. 
 * This below is from the book
 * but can be adapted further.
 */
typedef struct s_quad
{
	t_hittable  base;
	t_point3	q;
	t_vec3		u;
	t_vec3		v;
	t_rgb			rgb;
	t_color			color;
	union
	{
		t_metal			metal;
		t_lambertian 	lambertian_mat;
	};
	t_solid_color 	texture;
	t_material	*mat;
	double		d;
	t_vec3		normal;
	t_vec3		w;
	double		area;
	void		(*print)(const void* self);
}				t_quad;

void	quad_rgb(t_quad *qd, t_point3 q, t_vec3 u, t_vec3 v, t_rgb rgbcolor);
void 	quad_mat(t_quad *qd, t_point3 q, t_vec3 u, t_vec3 v, t_material *mat);
void	print_quad(const void *self);
bool	hit_quad(const void* self, const t_ray *r, t_interval ray_t,  t_hit_record *rec);
bool	is_interior(double a, double b, t_hit_record *rec);
double quad_pdf_value(const void *self, const t_point3 *orig, const t_vec3 *dir);
t_vec3 quad_random(const void *self, const t_point3 *orig);

#endif
