/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ralgaran <ralgaran@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 15:07:42 by ralgaran          #+#    #+#             */
/*   Updated: 2024/10/07 15:07:50 by ralgaran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cone.h"

/*
 * Inits an uncapped cone with an rgb color.
*/
void	cone_uncap_rgb(t_cone_uncap *c, t_init_params params)
{
	c->base.hit = hit_cone;
	c->base.random = obj_cone_random;
	c->base.pdf_value = obj_cone_pdf_value;
	c->apex = params.center;
	c->axis = unit_vector(vec3multscalar(params.normal, -1));
	c->radius = params.diam / 2;
	c->height = params.height;
	if (c->radius / c->height >= 0.7)
	{
		if (c->radius / c->height > 1)
			c->radius = c->height;
		c->radius *= 0.7;
	}
	c->color = rgb_to_color(params.rgbcolor);
	solid_color_init(&(c->texture), c->color);
	lambertian_init_tex(&(c->lambertian_mat), (t_texture *)&(c->texture));
	c->mat = (t_material *)&(c->lambertian_mat);
}

/*
 * Inits a capped cone with an rgb color.
*/
void	cone_rgb(t_cone *c, t_init_params params)
{
	t_point3	bottom_center;

	c->base.hit = hit_cone_cap;
	c->base.pdf_value = obj_pdf_value;
	c->base.random = obj_random;
	cone_uncap_rgb(&c->body, params);
	bottom_center = vec3add(params.center, \
		vec3multscalar(c->body.axis, params.height));
	// params.diam += 1.5;
	params.center = bottom_center;
	disk(&c->bottom, params);
}

/*
 * Inits an uncapped cone with a material.
*/
void	cone_uncap_mat(t_cone_uncap *c, t_init_params params)
{
	c->base.hit = hit_cone;
	c->base.random = obj_cone_random;
	c->base.pdf_value = obj_cone_pdf_value;
	c->apex = params.center;
	c->axis = unit_vector(vec3multscalar(params.normal, -1));
	c->radius = params.diam / 2;
	c->height = params.height;
	c->mat = params.mat;
}

/*
 * Inits a capped cone with an material.
*/
void	cone_mat(t_cone *c, t_init_params params)
{
	t_point3	bottom_center;

	c->base.hit = hit_cone_cap;
	c->base.pdf_value = obj_pdf_value;
	c->base.random = obj_random;
	cone_uncap_mat(&c->body, params);
	bottom_center = vec3add(params.center, \
		vec3multscalar(c->body.axis, params.height));
	// params.diam += 1;
	params.center = bottom_center;
	disk_mat(&c->bottom, params);
}
