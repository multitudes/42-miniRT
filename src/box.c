/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   box.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 11:40:26 by lbrusa            #+#    #+#             */
/*   Updated: 2024/10/02 13:46:12 by lbrusa           ###   ########.fr       */
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
	quad_rgb(&box->q1, point3(min.x, min.y, max.z), dx, dy, rgb);
	quad_rgb(&box->q2, point3(max.x, min.y, max.z), vec3negate(dz), dy, rgb);
	quad_rgb(&box->q3, point3(max.x, min.y, min.z), vec3negate(dx), dy, rgb);
	quad_rgb(&box->q4, point3(min.x, min.y, min.z), dz, dy, rgb);
	quad_rgb(&box->q5, point3(min.x, max.y, max.z), dx, vec3negate(dz), rgb);
	quad_rgb(&box->q6, point3(min.x, min.y, min.z), dx, dz, rgb);
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
void	box_rgb(t_box *box, t_point3 a, t_point3 b, t_rgb rgb)
{
	t_point3	min;
	t_point3	max;

	box->a = a;
	box->b = b;
	min = point3(fmin(a.x, b.x), fmin(a.y, b.y), fmin(a.z, b.z));
	max = point3(fmax(a.x, b.x), fmax(a.y, b.y), fmax(a.z, b.z));
	set_box_quad(box, min, max, rgb);
	box->rgb = rgb;
	solid_color_init(&(box->texture), box->color);
	lambertian_init_tex(&(box->lambertian_mat), (t_texture *)&(box->texture));
	box->mat = (t_material *)&(box->lambertian_mat);
	box->base.hit = hit_box;
	box->base.pdf_value = obj_pdf_value;
	box->base.random = obj_random;
	box->print = print_box;
}

void	box(t_box *box, t_point3 a, t_point3 b, t_material *mat)
{
	t_point3	min;
	t_point3	max;
	t_vec3		dx;
	t_vec3		dy;
	t_vec3		dz;

	box->a = a;
	box->b = b;
	min = point3(fmin(a.x, b.x), fmin(a.y, b.y), fmin(a.z, b.z));
	max = point3(fmax(a.x, b.x), fmax(a.y, b.y), fmax(a.z, b.z));
	dx = vec3(max.x - min.x, 0, 0);
	dy = vec3(0, max.y - min.y, 0);
	dz = vec3(0, 0, max.z - min.z);
	quad_mat(&box->q1, point3(min.x, min.y, max.z), dx, dy, mat);
	quad_mat(&box->q2, point3(max.x, min.y, max.z), vec3negate(dz), dy, mat);
	quad_mat(&box->q3, point3(max.x, min.y, min.z), vec3negate(dx), dy, mat);
	quad_mat(&box->q4, point3(min.x, min.y, min.z), dz, dy, mat);
	quad_mat(&box->q5, point3(min.x, max.y, max.z), dx, vec3negate(dz), mat);
	quad_mat(&box->q6, point3(min.x, min.y, min.z), dx, dz, mat);
	box->rgb = rgb(0, 0, 0);
	box->base.hit = hit_box;
	box->base.pdf_value = obj_pdf_value;
	box->base.random = obj_random;
	box->print = print_box;
}

void	print_box(const void *self)
{
	t_box	*box;

	box = (t_box *)self;
	printf("box\t\t%.f,%.f,%.f\t\t%.f,%.f,%.f\t\t%d,%d,%d\n", box->a.x,
		box->a.y, box->a.z, box->b.x, box->b.y, box->b.z, box->rgb.r,
		box->rgb.g, box->rgb.b);
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
