/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 14:57:19 by lbrusa            #+#    #+#             */
/*   Updated: 2024/09/06 15:08:35 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CYLINDER_H
# define CYLINDER_H

# include "vec3.h"
# include "hittable.h"
# include "color.h"

/**
 * @brief A cylinder is a hittable object
 * identifier: cy
 ∗ x,y,z coordinates of the center of the cylinder: 50.0,0.0,20.6
 ∗ 3d normalized vector of axis of cylinder. 
 * In range [-1,1] for each x,y,z axis:
 * 0.0,0.0,1.0
 ∗ the cylinder diameter: 14.2
 ∗ the cylinder height: 21.42
 ∗ R,G,B colors in range [0,255]: 10, 0, 255
 */
typedef struct	s_cylinder
{
	t_hittable  base;
	t_point3	center;
	t_vec3		axis;
	double		radius;
	double		height;
	t_color		color;
	t_rgb		rgb;
	void		(*print)(const void* self);
}				t_cylinder;

/*
 * a sort of initializer for a plane
 */
t_cylinder		cylinder(t_point3 center, t_vec3 axis, double diameter, double height, t_rgb color);
void		print_cylinder(const void *self);
/* if the ray hits the sphere, return the t value */
bool		hit_cyclinder(const void* self, const t_ray *r, t_interval closest, t_hit_record *rec);



#endif