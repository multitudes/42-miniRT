/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 10:52:56 by lbrusa            #+#    #+#             */
/*   Updated: 2024/09/06 11:42:20 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vec3.h"
#include "ray.h"
#include <stdbool.h>
#include "hittable.h"
#include "color.h"

typedef struct	s_sphere
{
	t_hittable  base;
	t_point3	center;
	double		radius;
	t_color		color;
	t_rgb		rgb;
	void		(*print)(const void* self);
}				t_sphere;

/*
 * a sort of initializer for a sphere
 */
t_sphere	sphere(t_point3 center, double diameter, t_rgb color);
void		print_sphere(const void *self);
/* if the ray hits the sphere, return the t value */
bool		hit_sphere(const void* self, const t_ray *r, t_interval closest, t_hit_record *rec);
