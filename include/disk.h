/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   disk.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 09:07:01 by lbrusa            #+#    #+#             */
/*   Updated: 2024/09/30 09:27:59 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DISK_H
# define DISK_H

# include "color.h"
# include "hittable.h"
# include "interval.h"
# include "material.h"
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

void					disk(t_disk *d, t_point3 center, t_vec3 normal,
							double diam, t_rgb rgbcolor);
void					disk_mat(t_disk *d, t_point3 center, t_vec3 normal,
							double diam, t_material *mat);
void					print_disk(const void *self);
bool					hit_disk(const void *self, const t_ray *r,
							t_interval ray_t, t_hit_record *rec);
bool					is_interior_disk(double a, double b, t_hit_record *rec,
							t_vec3 u, t_vec3 v);
double					disk_pdf_value(const void *self, const t_point3 *orig,
							const t_vec3 *dir);
t_vec3					disk_random(const void *self, const t_point3 *orig);

#endif
