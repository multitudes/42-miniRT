/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   disk.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 09:07:01 by lbrusa            #+#    #+#             */
/*   Updated: 2024/10/10 19:38:39 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DISK_H
# define DISK_H

# include "color.h"
# include "hittable.h"
# include "interval.h"
# include "material.h"
# include "quad.h"
# include "ray.h"
# include "vec3.h"

typedef struct s_disk
{
	t_hittable			base;
	t_point3			center;
	double				radius;
	t_vec3				u;
	t_vec3				v;
	double				d;
	t_vec3				normal;
	t_vec3				w;
	t_rgb				rgb;
	t_color				color;
	t_rgb				rgbcolor;
	union
	{
		t_metal			metal;
		t_lambertian	lambertian_mat;
	};
	t_solid_color		texture;
	t_material			*mat;
	void				(*print)(const void *self);
}						t_disk;

void					disk(t_disk *d, t_init_params params);
void					disk_mat(t_disk *d, t_init_params params);
void					print_disk(const void *self);
bool					hit_disk(const void *self, const t_ray *r,
							t_interval ray_t, t_hit_record *rec);
double					disk_pdf_value(const void *self, const t_point3 *orig,
							const t_vec3 *dir);
t_vec3					disk_random(const void *self, const t_point3 *orig);

#endif
