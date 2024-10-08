/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 14:57:19 by lbrusa            #+#    #+#             */
/*   Updated: 2024/10/08 18:09:53 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CYLINDER_H
# define CYLINDER_H

# include "color.h"
# include "disk.h"
# include "hittable.h"
# include "material.h"
#include "quad.h"
# include "vec3.h"

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
typedef struct s_cylinder
{
	t_hittable			base;
	t_point3			center;
	t_vec3				axis;
	double				radius;
	double				height;
	double				min;
	double				max;
	t_color				color;
	t_rgb				rgb;
	t_material			*mat;
	union
	{
		t_metal			metal;
		t_lambertian	lambertian_mat;
	};
	t_solid_color		texture;
	void				(*print)(const void *self);
}						t_cylinder;

typedef struct s_cylinder_capped
{
	t_hittable			base;
	t_cylinder			cylinder_uncapped;
	t_disk				top;
	t_disk				bottom;
	void				(*print)(const void *self);
}						t_cylinder_capped;

typedef struct s_cyl_utils
{
	t_cylinder		*cyl;
	const t_ray		*r;
	double			closest_t;
	t_vec3			closest_point;
	t_vec3			normal;
}			t_cyl_utils;

void					cylinder_uncapped(t_cylinder *c, t_init_params params);
void					cylinder_mat_uncapped(t_cylinder *c, t_init_params params);
void					cylinder_capped(t_cylinder_capped *c, t_init_params params);
void					cylinder_mat_capped(t_cylinder_capped *c, t_init_params params);
bool					hit_cylinder_capped(const void *self, const t_ray *r,
							t_interval closest, t_hit_record *rec);
void					print_cylinder(const void *self);
void					print_cylinder_capped(const void *self);
bool					hit_cylinder(const void *self, const t_ray *r,
							t_interval closest, t_hit_record *rec);
double					obj_cylinder_pdf_value(const void *self,
							const t_point3 *orig, const t_vec3 *dir);
t_vec3					obj_cylinder_random(const void *self,
							const t_point3 *orig);
void					get_cylinder_uncappedv(t_vec3 normal, double uv[2]);

#endif
