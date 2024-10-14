/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 10:52:56 by lbrusa            #+#    #+#             */
/*   Updated: 2024/10/14 11:03:48 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SPHERE_H
# define SPHERE_H

# include "color.h"
# include "hittable.h"
# include "material.h"
# include "quad.h"
# include "ray.h"
# include "texture.h"
# include "vec3.h"
# include <stdbool.h>

typedef struct s_sphere
{
	t_hittable				base;
	t_point3				center;
	double					radius;
	t_color					color;
	t_rgb					rgb;
	t_material				*mat;
	union
	{
		t_lambertian		lambertian_mat;
		t_metal				metal;
	};
	union
	{
		t_checker_texture	checker;
		t_solid_color		texture;
	};
	t_img_texture			img_texture;
	void					(*print)(const void *self);
}							t_sphere;

void						sphere(t_sphere *s, t_init_params params);
void						sphere_mat(t_sphere *s, t_init_params params);
bool						hit_sphere(const void *self, const t_ray *r,
								t_interval closest, t_hit_record *rec);
void						get_sphere_uv(t_vec3 normal, double uv[2]);
double						obj_sphere_pdf_value(const void *self,
								const t_point3 *orig, const t_vec3 *dir);
t_vec3						obj_sphere_random(const void *self,
								const t_point3 *orig);
t_vec3						random_to_sphere(double radius,
								double distance_squared);

#endif
