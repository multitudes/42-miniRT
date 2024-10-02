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

# include "color.h"
# include "disk.h"
# include "hittable.h"
# include "material.h"
# include "texture.h"
# include "vec3.h"
# include <stdbool.h>

typedef struct
{
	/* like cylinder: */
	t_hittable		base;
	t_point3		apex;		// the top of the cone
	t_vec3			axis;		// the axis
	double			radius;		// radius of the base
	double			height;		// height of the whole cone axis
	
	/* maybe dont need theese */
	double			angle;		// half angle
	double			cosa;		// 
	
	
	
	t_color			color;
	t_material		*mat;
	union
	{
		t_metal			metal;
		t_lambertian	lambertian_mat;
	};
	t_solid_color	texture;
}		t_cone_uncap;

typedef struct
{
	t_hittable		base;
	t_cone_uncap	body;
	t_disk			bottom;
}					t_cone;

/* init function */
void	cone_rgb(t_cone *c, t_point3 center, t_vec3 axis, double diam, double height, \
	t_rgb rgbcolor);
void	cone_mat(t_cone *c, t_point3 center, t_vec3 axis, double diam, double height, \
	t_material *mat);

/* hit function */
bool	hit_cone(const void *self, const t_ray *ray, t_interval closest, t_hit_record *rec);

#endif
