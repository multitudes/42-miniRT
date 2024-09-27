/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ralgaran <ralgaran@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 16:06:01 by ralgaran          #+#    #+#             */
/*   Updated: 2024/09/16 16:06:02 by ralgaran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONE_H
# define CONE_H

#include "color.h"
#include "disk.h"
# include "hittable.h"
#include "material.h"
#include "texture.h"
# include "vec3.h"
#include <stdbool.h>

typedef struct
{
	t_hittable		base;
	t_point3		apex;
	t_vec3			axis;
	double			angle;
	double			height;
	double			min;
	t_color			color;
	t_material		*mat;
	t_disk			bottom;
	union
	{
		t_metal			metal;
		t_lambertian	lambertian_mat;
	};
	t_solid_color	texture;
}					t_cone;

/* init function */
void	cone(t_cone *c, t_point3 apex, t_vec3 axis, double angle, double height, \
	t_rgb rgbcolor);

/* hit function */
bool	hit_cone(const void *self, const t_ray *ray, t_interval closest, t_hit_record *rec);

#endif
