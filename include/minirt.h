/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/22 16:34:23 by lbrusa            #+#    #+#             */
/*   Updated: 2024/09/19 17:40:55 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

#include <limits.h>
#include <MLX42/MLX42.h>
#include "camera.h"
#include "ambient.h"
#include "sphere.h"
#include "plane.h"
#include "cylinder.h"


# define TRUE 1
# define FALSE 0

typedef struct 	s_mrt 
{
	void		*mlx;
	void		*win_ptr;
	mlx_image_t	*image;
	bool needs_render;

	t_camera cam;
	t_ambient ambient_light;

	t_sphere spheres[100];
	t_plane planes[100];
	t_cylinder cylinders[100];

	// hittable list
	t_hittablelist world;
	t_hittablelist lights;
	// render function
	void (*renderscene)(struct s_mrt *, const t_hittablelist *, const t_hittablelist *);
	mlx_image_t	*sobel_img;
	// t_viewport	viewport;
	// t_pixel		pixel;
}				t_mrt;



#endif
