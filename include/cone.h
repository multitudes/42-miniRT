/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 16:06:01 by ralgaran          #+#    #+#             */
/*   Updated: 2024/09/30 09:25:09 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONE_H
# define CONE_H

# include "color.h"
# include "hittable.h"
# include "material.h"
# include "vec3.h"
# include <stdbool.h>

typedef struct s_cone
{
	t_hittable			base;
	t_point3			apex;
	t_vec3				axis;
	double				angle;
	double				height;
	t_rgb				rgbcolor;
	t_material			*mat;
	union
	{
		t_metal			metal;
		t_lambertian	lambertian_mat;
	};
}						t_cone;

/* init function  (think about norminrtte! :))*/
void					cone(t_cone *c, t_point3 apex, t_vec3 axis,
							double angle, double height, t_rgb rgbcolor);

/* hit function */
bool					hit_cone(const void *self, const t_ray *ray,
							t_interval closest, t_hit_record *rec);

#endif
