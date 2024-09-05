/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 10:52:56 by lbrusa            #+#    #+#             */
/*   Updated: 2024/09/04 18:14:00 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vec3.h"
#include "ray.h"
#include <stdbool.h>
#include "hittable.h"

typedef struct	s_sphere
{
	t_hittable  base;
	t_point3	center;
	double		radius;
}				t_sphere;

/*
 * a sort of initializer for a sphere
 */
t_sphere	sphere(t_point3 center, double radius);

/* if the ray hits the sphere, return the t value */
bool		hit_sphere(const void* self, const t_ray *r, t_interval closest, t_hit_record *rec);
