/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 15:07:07 by lbrusa            #+#    #+#             */
/*   Updated: 2024/10/08 18:11:29 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cylinder.h"
#include "vec3.h"
#include "ray.h"
#include "color.h"
#include <stdio.h>
#include "material.h"
#include "utils.h"

/**
 * @brief: initializer for a cylinder uncapped standard material
 *
 * the rt file will have the following format:
 * cy 	50.0,0.0,20.6     0.0,0.0,1.0    14.2 21.42 	10,0,255
 * x,y,z coordinates of the center of the cylinder
 * 3d normalized vector of axis of cylinder. In range [-1,1] for each x,y,z axis
 * the cylinder diameter
 * the cylinder height
 * R,G,B colors in range [0,255]
*/
void		cylinder_uncapped(t_cylinder *c, t_init_params params)
{
	c->base.hit = hit_cylinder;
	c->base.pdf_value = obj_cylinder_pdf_value;
	c->base.random = obj_cylinder_random;
	c->center = params.center;
	c->axis = unit_vector(params.normal);
	c->radius = params.diam / 2;
	c->height = params.height;
	c->min = -params.height / 2;
	c->max = params.height / 2;
	c->rgb = params.rgbcolor;
	c->color = rgb_to_color(params.rgbcolor);
	solid_color_init(&(c->texture), c->color);
	lambertian_init_tex(&(c->lambertian_mat), (t_texture*)&(c->texture));
 	c->mat = (t_material*)&(c->lambertian_mat);
	c->print = &print_cylinder;
}

/**
 * @brief: initializer for a cylinder uncapped with a material
 *
 * the rt file will have the following format:
 *
 */
void		cylinder_mat_uncapped(t_cylinder *c, t_init_params params)
{
	c->base.hit = hit_cylinder;
	c->base.pdf_value = obj_cylinder_pdf_value;
	c->base.random = obj_cylinder_random;
	c->center = params.center;
	c->axis = unit_vector(params.normal);
	c->radius = params.diam / 2;
	c->height = params.height;
	c->min = -params.height / 2;
	c->max = params.height / 2;
	c->mat = params.mat;
	c->print = &print_cylinder;
}

/**
 * @brief: initializer for a cylinder capped with a standard material
 *
 * the rt file will have the following format:
 *
 */
void cylinder_capped(t_cylinder_capped *c, t_init_params params)
{
	c->base.hit = hit_cylinder_capped;
	c->base.pdf_value = obj_pdf_value;
	c->base.random = obj_random;
	cylinder_uncapped(&c->cylinder_uncapped, params);

	t_vec3 axis = unit_vector(params.normal);
	t_point3 top_center = vec3add(params.center, vec3multscalar(axis, params.height / 2));
    t_point3 bottom_center = vec3add(params.center, vec3multscalar(axis, -params.height / 2));
    params.center = top_center;
	disk(&c->top, params);
	params.center = bottom_center;
	disk(&c->bottom, params);
	c->print = &print_cylinder_capped;
}

void cylinder_mat_capped(t_cylinder_capped *c, t_init_params params)
{
	c->base.hit = hit_cylinder_capped;
	c->base.pdf_value = obj_pdf_value;
	c->base.random = obj_random;
	cylinder_mat_uncapped(&c->cylinder_uncapped, params);
	
	t_vec3 axis = unit_vector(params.normal);
    t_point3 top_center = vec3add(params.center, vec3multscalar(axis, params.height / 2));
    t_point3 bottom_center = vec3add(params.center, vec3multscalar(axis, -(params.height / 2)));
    params.center = top_center;
	disk_mat(&c->top, params);
	params.center = bottom_center;
	disk_mat(&c->bottom, params);
	c->print = &print_cylinder_capped;
}


/**
 * @brief print the cylinder capped object
 *
 * The print will just print the cylinder which is a part of the capped cylinder
 */
void print_cylinder_capped(const void *self)
{
	const t_cylinder_capped *c = (const t_cylinder_capped *)self;
	c->cylinder_uncapped.print(&c->cylinder_uncapped);
}

/**
 * @brief print the cylinder object
 * cy 50.0,0.0,20.6 	 0.0,0.0,1.0  	14.2	 21.42  	10,0,255
 */
void		print_cylinder(const void *self)
{
	const t_cylinder *c = (const t_cylinder *)self;
	printf("cy\t%.f,%.f,%.f\t\t%.f,%.f,%.f\t\t%.f\t%.f\t%d,%d,%d\n",
	c->center.x, c->center.y, c->center.z, c->axis.x, c->axis.y, c->axis.z, c->radius * 2, c->height, c->rgb.r, c->rgb.g, c->rgb.b);

}

