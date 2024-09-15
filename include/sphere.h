/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 10:52:56 by lbrusa            #+#    #+#             */
/*   Updated: 2024/09/07 16:59:37 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SPHERE_H
# define SPHERE_H

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
	t_lambertian 	lambertian_mat;
	t_solid_color 	texture;
}					t_sphere;


void	sphere(t_sphere *s, t_point3 center, double diameter, t_rgb color);
t_sphere 	sphere_mat(t_point3 center, double diameter, t_rgb rgbcolor, t_material *mat);
/* if the ray hits the sphere, return the t value */
bool		hit_sphere(const void* self, const t_ray *r, t_interval closest, t_hit_record *rec);
void		get_sphere_uv(t_vec3 normal, double* u, double* v);


#endif
