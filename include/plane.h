/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 14:53:24 by lbrusa            #+#    #+#             */
/*   Updated: 2024/10/10 12:30:17 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PLANE_H
# define PLANE_H

# include "color.h"
# include "hittable.h"
# include "material.h"
# include "texture.h"
#include "utils.h"
# include "vec3.h"

# define PLANE_MAX 1000

/**
 * @brief a plane object
 * identifier: pl
 ∗ x,y,z coordinates of a point in the plane: 0.0,0.0,-10.0
 ∗ 3d normalized normal vector. In range [-1,1] for each x,y,z axis: 0.0,1.0,0.0
 ∗ R,G,B colors in range [0-255]: 0,0,225
 */
typedef struct s_plane
{
	t_hittable				base;
	t_point3				q;
	double					d;
	t_vec3					u;
	t_vec3					v;
	t_vec3					normal;
	t_rgb					rgb;
	t_color					color;
	union
	{
		t_metal				metal;
		t_lambertian		lambertian_mat;
	};
	union
	{
		t_solid_color		solid;
		t_checker_texture	checker;
	};
	t_material				*mat;
	void					(*print)(const void *self);
}							t_plane;

/*
 * a sort of initializer for a plane
 */
void						plane(t_plane *pl, t_init_params params);
void						plane_mat(t_plane *pl, t_init_params params);
void						print_plane(const void *self);
/* if the ray hits the sphere, return the t value */
bool						hit_plane(const void *self, const t_ray *r,
								t_interval closest, t_hit_record *rec);
double						plane_pdf_value(const void *self,
								const t_point3 *orig, const t_vec3 *dir);
t_vec3						plane_random(const void *self,
								const t_point3 *orig);

#endif
