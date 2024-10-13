/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   box.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 11:40:26 by lbrusa            #+#    #+#             */
/*   Updated: 2024/10/13 14:05:28 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "box.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "quad.h"
#include "vec3.h"
#include <stdio.h>

void	set_box_quad(t_box *box, t_point3 min, t_point3 max, t_rgb rgb)
{
	t_vec3	dx;
	t_vec3	dy;
	t_vec3	dz;

	dx = vec3(max.x - min.x, 0, 0);
	dy = vec3(0, max.y - min.y, 0);
	dz = vec3(0, 0, max.z - min.z);
	quad_rgb(&box->q1, (t_init_params){.center = point3(min.x, min.y, max.z),
		.side1 = dx, .side2 = dy, .rgbcolor = rgb});
	quad_rgb(&box->q2, (t_init_params){.center = point3(max.x, min.y, max.z),
		.side1 = vec3negate(dz), .side2 = dy, .rgbcolor = rgb});
	quad_rgb(&box->q3, (t_init_params){.center = point3(max.x, min.y, min.z),
		.side1 = vec3negate(dx), .side2 = dy, .rgbcolor = rgb});
	quad_rgb(&box->q4, (t_init_params){.center = point3(min.x, min.y, min.z),
		.side1 = dz, .side2 = dy, .rgbcolor = rgb});
	quad_rgb(&box->q5, (t_init_params){.center = point3(min.x, max.y, max.z),
		.side1 = dx, .side2 = vec3negate(dz), .rgbcolor = rgb});
	quad_rgb(&box->q6, (t_init_params){.center = point3(min.x, min.y, min.z),
		.side1 = dx, .side2 = dz, .rgbcolor = rgb});
}

/**
 * @brief Creates a box with a rgbcolor
 *
 * @param t_box box
 * @param t_point3 a
 * @param t_point3 b
 * @param t_rgb rgb
 *
 * this is the default initialiser for a box which takes a point a and b
 * and a rgb color.
 *
 */
void	box_rgb(t_box *box, t_init_params params)
{
	t_point3	min;
	t_point3	max;
	t_point3	a;
	t_point3	b;

	a = params.a;
	b = params.b;
	box->a = a;
	box->b = b;
	min = point3(fmin(a.x, b.x), fmin(a.y, b.y), fmin(a.z, b.z));
	max = point3(fmax(a.x, b.x), fmax(a.y, b.y), fmax(a.z, b.z));
	set_box_quad(box, min, max, params.rgbcolor);
	box->rgb = params.rgbcolor;
	solid_color_init(&(box->texture), box->color);
	lambertian_init_tex(&(box->lambertian_mat), (t_texture *)&(box->texture));
	box->mat = (t_material *)&(box->lambertian_mat);
	box->base.hit = hit_box;
	box->base.pdf_value = obj_pdf_value;
	box->base.random = obj_random;
}

void	box_minimal_init(t_box *box, t_init_params p)
{
	box->a = p.a;
	box->b = p.b;
	box->rgb = rgb(0, 0, 0);
	box->base.hit = hit_box;
	box->base.pdf_value = obj_pdf_value;
	box->base.random = obj_random;
}

void	box(t_box *box, t_init_params p)
{
	t_point3	min;
	t_point3	max;
	t_vec3		dx;
	t_vec3		dy;
	t_vec3		dz;

	box_minimal_init(box, p);
	min = point3(fmin(p.a.x, p.b.x), fmin(p.a.y, p.b.y), fmin(p.a.z, p.b.z));
	max = point3(fmax(p.a.x, p.b.x), fmax(p.a.y, p.b.y), fmax(p.a.z, p.b.z));
	dx = vec3(max.x - min.x, 0, 0);
	dy = vec3(0, max.y - min.y, 0);
	dz = vec3(0, 0, max.z - min.z);
	quad_mat(&box->q1, (t_init_params){.center = point3(min.x, min.y, max.z),
		.side1 = dx, .side2 = dy, .mat = p.mat});
	quad_mat(&box->q2, (t_init_params){.center = point3(max.x, min.y, max.z),
		.side1 = vec3negate(dz), .side2 = dy, .mat = p.mat});
	quad_mat(&box->q3, (t_init_params){.center = point3(max.x, min.y, min.z),
		.side1 = vec3negate(dx), .side2 = dy, .mat = p.mat});
	quad_mat(&box->q4, (t_init_params){.center = point3(min.x, min.y, min.z),
		.side1 = dz, .side2 = dy, .mat = p.mat});
	quad_mat(&box->q5, (t_init_params){.center = point3(min.x, max.y, max.z),
		.side1 = dx, .side2 = vec3negate(dz), .mat = p.mat});
	quad_mat(&box->q6, (t_init_params){.center = point3(min.x, min.y, min.z),
		.side1 = dx, .side2 = dz, .mat = p.mat});
}

bool	hit_box(const void *self, const t_ray *r, t_interval ray_t,
		t_hit_record *rec)
{
	t_box			*box;
	t_hittablelist	box_hittable_list;
	t_hittable		*list[6];

	box = (t_box *)self;
	list[0] = (t_hittable *)(&box->q1);
	list[1] = (t_hittable *)(&box->q2);
	list[2] = (t_hittable *)(&box->q3);
	list[3] = (t_hittable *)(&box->q4);
	list[4] = (t_hittable *)(&box->q5);
	list[5] = (t_hittable *)(&box->q6);
	box_hittable_list = hittablelist(list, 6);
	return (hit_objects(&box_hittable_list, r, ray_t, rec));
}
