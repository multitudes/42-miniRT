/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   box.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 11:40:26 by lbrusa            #+#    #+#             */
/*   Updated: 2024/09/15 12:29:56 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "box.h"
#include "vec3.h"
#include "quad.h"
#include "material.h"
#include "hittable_list.h"
#include "hittable.h"

t_box box_rgb(t_point3 a, t_point3 b, t_rgb rgb)
{
	t_box box;
    box.a = a;
    box.b = b;
	t_point3 min = point3(fmin(a.x, b.x), fmin(a.y, b.y), fmin(a.z, b.z));
    t_point3 max = point3(fmax(a.x, b.x), fmax(a.y, b.y), fmax(a.z, b.z));

    t_vec3 dx = vec3(max.x - min.x, 0, 0);
    t_vec3 dy = vec3(0, max.y - min.y, 0);
    t_vec3 dz = vec3(0, 0, max.z - min.z);

    box.q1 = quad_rgb(point3(min.x, min.y, max.z),  dx,  dy, rgb); // front
    box.q2 = quad_rgb(point3(max.x, min.y, max.z), vec3negate(dz),  dy, rgb); // right
    box.q3 = quad_rgb(point3(max.x, min.y, min.z), vec3negate(dx),  dy, rgb); // back
    box.q4 = quad_rgb(point3(min.x, min.y, min.z),  dz,  dy, rgb); // left
    box.q5 = quad_rgb(point3(min.x, max.y, max.z),  dx, vec3negate(dz), rgb); // top
    box.q6 = quad_rgb(point3(min.x, min.y, min.z),  dx,  dz, rgb); // bottom
    box.rgb = rgb;

// i use the color to create a texture
	solid_color_init(&(box.texture), box.color);
	// i init the lambertian material with the texture
	lambertian_init_tex(&(box.lambertian_mat), (t_texture*)&(box.texture));
	// i assign the material to the sphere as a pointer
	// the pointer will contain the scatter function for the material
	// which will be passed to the t_record struct when hit
 	box.mat = (t_material*)&(box.lambertian_mat); 

	box.base.hit = hit_box;
    box.base.pdf_value = obj_pdf_value;
    box.base.random = obj_random;
    box.print = print_box;  

	return (box);
}

t_box box(t_point3 a, t_point3 b, t_material *mat)
{
	t_box box;
    box.a = a;
    box.b = b;
	t_point3 min = point3(fmin(a.x, b.x), fmin(a.y, b.y), fmin(a.z, b.z));
    t_point3 max = point3(fmax(a.x, b.x), fmax(a.y, b.y), fmax(a.z, b.z));

    t_vec3 dx = vec3(max.x - min.x, 0, 0);
    t_vec3 dy = vec3(0, max.y - min.y, 0);
    t_vec3 dz = vec3(0, 0, max.z - min.z);

    box.q1 = quad(point3(min.x, min.y, max.z),  dx,  dy, mat); // front
    box.q2 = quad(point3(max.x, min.y, max.z), vec3negate(dz),  dy, mat); // right
    box.q3 = quad(point3(max.x, min.y, min.z), vec3negate(dx),  dy, mat); // back
    box.q4 = quad(point3(min.x, min.y, min.z),  dz,  dy, mat); // left
    box.q5 = quad(point3(min.x, max.y, max.z),  dx, vec3negate(dz), mat); // top
    box.q6 = quad(point3(min.x, min.y, min.z),  dx,  dz, mat); // bottom
    box.rgb = rgb(0, 0, 0);
	box.base.hit = hit_box;
    box.base.pdf_value = obj_pdf_value;
    box.base.random = obj_random;
    box.print = print_box;  

	return (box);
}


// void create_box(t_box *box, t_point3 a, t_point3 b, t_material *mat)
// {
//   	t_point3 min = point3(fmin(a.x, b.x), fmin(a.y, b.y), fmin(a.z, b.z));
//     t_point3 max = point3(fmax(a.x, b.x), fmax(a.y, b.y), fmax(a.z, b.z));

//     t_vec3 dx = vec3(max.x - min.x, 0, 0);
//     t_vec3 dy = vec3(0, max.y - min.y, 0);
//     t_vec3 dz = vec3(0, 0, max.z - min.z);

//     box->q1 = quad(point3(min.x, min.y, max.z),  dx,  dy, mat); // front
//     box->q2 = quad(point3(max.x, min.y, max.z), vec3negate(dz),  dy, mat); // right
//     box->q3 = quad(point3(max.x, min.y, min.z), vec3negate(dx),  dy, mat); // back
//     box->q4 = quad(point3(min.x, min.y, min.z),  dz,  dy, mat); // left
//     box->q5 = quad(point3(min.x, max.y, max.z),  dx, vec3negate(dz), mat); // top
//     box->q6 = quad(point3(min.x, min.y, min.z),  dx,  dz, mat); // bottom
// }
void    print_box(const void *self)
{
    t_box *box = (t_box *)self;
    printf("box\t\t%.f,%.f,%.f\t\t%.f,%.f,%.f\t\t%d,%d,%d\n", 
    box->a.x, box->a.y, box->a.z,box->b.x, box->b.y, box->b.z, box->rgb.r, box->rgb.g, box->rgb.b);
}

bool hit_box(const void* self, const t_ray *r, t_interval ray_t,  t_hit_record *rec)
{
	t_box *box = (t_box *)self;
	
	t_hittablelist box_hittable_list;

	t_hittable *list[6];
	
	// add to list
	list[0] = (t_hittable*)(&box->q1);
	list[1] = (t_hittable*)(&box->q2);
	list[2] = (t_hittable*)(&box->q3);
	list[3] = (t_hittable*)(&box->q4);
	list[4] = (t_hittable*)(&box->q5);
	list[5] = (t_hittable*)(&box->q6);

	box_hittable_list = hittablelist(list, 6);
	


	return hit_objects(&box_hittable_list, r, ray_t, rec);
}