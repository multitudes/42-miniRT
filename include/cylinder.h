/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 14:57:19 by lbrusa            #+#    #+#             */
/*   Updated: 2024/09/16 17:14:08 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CYLINDER_H
# define CYLINDER_H

# include "vec3.h"
# include "hittable.h"
# include "color.h"
#include "material.h"
#include "interval.h"

/**
 * @brief A cylinder is a hittable object
 * identifier: cy
 ∗ x,y,z coordinates of the center of the cylinder: 50.0,0.0,20.6
 ∗ 3d normalized vector of axis of cylinder.
 * In range [-1,1] for each x,y,z axis:
 * 0.0,0.0,1.0
 ∗ the cylinder diameter: 14.2
 ∗ the cylinder height: 21.42
 ∗ R,G,B colors in range [0,255]: 10, 0, 255
 */
typedef struct	s_cylinder
{
	t_hittable  	base;
	t_point3		center;
	t_vec3			axis;
	double			radius;
	double			height;
	t_color			color;
	t_rgb			rgb;
	t_material		*mat;
	t_lambertian 	lambertian_mat;
	t_solid_color 	texture;
	void			(*print)(const void* self);
}				t_cylinder;

void		cylinder_u(t_cylinder *c, t_point3 center, t_vec3 axis, double diameter, double height, t_rgb rgbcolor);
void		cylinder_mat_u(t_cylinder *c, t_point3 center, t_vec3 axis, double diameter, double height, t_material *mat);
void		print_cylinder(const void *self);
bool		hit_cylinder(const void* self, const t_ray *r, t_interval closest, t_hit_record *rec);
double 		obj_cylinder_pdf_value(const void *self, const t_point3 *orig, const t_vec3 *dir);
t_vec3 		obj_cylinder_random(const void *self, const t_point3 *orig);
void		get_cylinder_uv(t_vec3 normal, double* u, double* v); 

#endif
