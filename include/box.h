/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   box.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 11:38:14 by lbrusa            #+#    #+#             */
/*   Updated: 2024/09/30 09:17:25 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOX_H
# define BOX_H

# include "color.h"
# include "hittable_list.h"
# include "material.h"
# include "quad.h"
# include "ray.h"
# include "vec3.h"

typedef struct s_box
{
	t_hittable			base;
	t_point3			a;
	t_point3			b;
	t_quad				q1;
	t_quad				q2;
	t_quad				q3;
	t_quad				q4;
	t_quad				q5;
	t_quad				q6;
	t_rgb				rgb;
	t_color				color;
	union
	{
		t_metal			metal;
		t_lambertian	lambertian_mat;
	};
	t_solid_color		texture;
	t_material			*mat;
	void				(*print)(const void *self);
}						t_box;

void					box_rgb(t_box *box, t_point3 a, t_point3 b, t_rgb rgb);
void					box(t_box *box, t_point3 a, t_point3 b,
							t_material *mat);
void					print_box(const void *self);
bool					hit_box(const void *self, const t_ray *r,
							t_interval ray_t, t_hit_record *rec);

#endif
