/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 14:53:24 by lbrusa            #+#    #+#             */
/*   Updated: 2024/09/06 15:15:38 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PLANE_H
# define PLANE_H

# include "vec3.h"
# include "hittable.h"
# include "color.h"

/**
 * @brief a plane object
 * identifier: pl
 ∗ x,y,z coordinates of a point in the plane: 0.0,0.0,-10.0
 ∗ 3d normalized normal vector. In range [-1,1] for each x,y,z axis: 0.0,1.0,0.0
 ∗ R,G,B colors in range [0-255]: 0,0,225
 * 
 */
typedef struct s_plane 
{
	t_hittable  base;
	t_point3	point;
	t_vec3		normal;
	t_color		color;
	t_rgb		rgb;
	void		(*print)(const void* self);
}				t_plane;

/*
 * a sort of initializer for a plane
 */
t_plane		plane(t_point3 center, t_vec3 normal, t_rgb color);
void		print_plane(const void *self);
/* if the ray hits the sphere, return the t value */
bool		hit_plane(const void* self, const t_ray *r, t_interval closest, t_hit_record *rec);


#endif