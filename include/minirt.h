/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/22 16:34:23 by lbrusa            #+#    #+#             */
/*   Updated: 2024/09/06 15:14:02 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

#include <limits.h>
#include <MLX42/MLX42.h>
#include <math.h>
#include "camera.h"
#include "ambient.h"
#include "sphere.h"
#include "plane.h"
#include "cylinder.h"
#include "vec3.h"

#define IMAGE_WIDTH 400
# define TRUE 1
# define FALSE 0

typedef struct 	s_mrt
{
	void		*mlx;
	void		*win_ptr;
	mlx_image_t	*image;

	t_camera cam;
	t_ambient ambient_light;

	t_sphere spheres[100];
	t_plane planes[100];
	t_cylinder cylinders[100];

	// t_viewport	viewport;
	// t_pixel		pixel;
}				t_mrt;

typedef struct
{
	t_vec3	center;
	double	brightness;
	t_color	color;
}			t_light;


typedef struct
{
	t_ambient	ambient;
	t_camera	camera;
	t_light		light;
	t_sphere	spheres[100];
	t_plane		planes[100];
	t_cylinder	cylinder[100];
}			t_objects;

void	parse_input(char *filename, t_objects *obj);

#endif
