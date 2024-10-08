/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 16:06:01 by ralgaran          #+#    #+#             */
/*   Updated: 2024/10/08 18:04:46 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONE_H
# define CONE_H

# include "disk.h"
# include "hittable.h"
# include "hittable_list.h"
# include "material.h"
# include "texture.h"
# include "vec3.h"
# include "utils.h"
# include <stdbool.h>

/* like cylinder: */
typedef struct s_cone_uncap	
{
	t_hittable			base;
	t_point3			apex;
	t_vec3				axis;
	double				radius;
	double				height;
	t_color				color;
	t_material			*mat;
	union
	{
		t_metal			metal;
		t_lambertian	lambertian_mat;
	};
	t_solid_color		texture;
}						t_cone_uncap;

typedef struct s_cone
{
	t_hittable			base;
	t_cone_uncap		body;
	t_disk				bottom;
}						t_cone;

/* util struct for hit_cone() */
typedef struct s_cone_utils
{
	t_cone_uncap	*cone;
	const t_ray		*r;
	double			closest_t;
	t_vec3			closest_point;
	t_vec3			normal;
}			t_cone_utils;

/* init functions */
void	cone_uncap_rgb(t_cone_uncap *c, t_init_params params);
void	cone_rgb(t_cone *c, t_init_params params);
void	cone_uncap_mat(t_cone_uncap *c, t_init_params params);
void	cone_mat(t_cone *c, t_init_params params);

/* hit function */
bool	hit_cone(const void *self, const t_ray *ray, \
	t_interval closest, t_hit_record *rec);

/* other utils */
bool	hit_cone_cap(const void *self, const t_ray *r, t_interval closest, \
	t_hit_record *rec);
double	obj_cone_pdf_value(const void *self, const t_point3 *orig, \
	const t_vec3 *dir);
t_vec3	obj_cone_random(const void *self, const t_point3 *orig);

#endif
