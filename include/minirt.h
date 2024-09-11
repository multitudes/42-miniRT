/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/22 16:34:23 by lbrusa            #+#    #+#             */
/*   Updated: 2024/09/10 19:28:34 by lbrusa           ###   ########.fr       */
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

# define IMAGE_WIDTH 400
# define TRUE 1
# define FALSE 0
# define OBJECT_COUNT 100

/* struct that we didnt already have - used in the parser */
typedef struct
{
	t_vec3	center;
	double	brightness;
	t_rgb	color;
}			t_light;

/* added for the parser */
typedef struct
{
	t_ambient	ambient;
	t_camera	camera;
	t_light		lights[OBJECT_COUNT];
	t_sphere	spheres[OBJECT_COUNT];
	t_plane		planes[OBJECT_COUNT];
	t_cylinder	cylinders[OBJECT_COUNT];
	// t_cones		cones[OBJECT_COUNT];
	t_hittable *hit_list[OBJECT_COUNT * 5];
	int 		hit_idx;
}			t_objects;

typedef struct 	s_mrt
{
	void		*mlx;
	void		*win_ptr;
	mlx_image_t	*image;

	t_objects	objects;

	// hittable list
	t_hittablelist world;
	// render function
	    void (*renderscene)(struct s_mrt *, const t_hittablelist *);

	// t_viewport	viewport;
	// t_pixel		pixel;
}				t_mrt;

void	parse_input(char *filename, t_objects *obj);

#endif
