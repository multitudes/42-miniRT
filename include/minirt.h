/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/22 16:34:23 by lbrusa            #+#    #+#             */
/*   Updated: 2024/09/28 16:57:37 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

#include <limits.h>
#include <MLX42/MLX42.h>
#include "camera.h"
#include "disk.h"
#include "hittable.h"
#include "material.h"
#include "quad.h"
#include "sphere.h"
#include "plane.h"
#include "cylinder.h"
#include "texture.h"
#include "triangle.h"
#include "cone.h"
#include "box.h"

# define TRUE 1
# define FALSE 0
# define OBJECT_COUNT 20
# define SPHERES_COUNT 1000
# define CHECKER_SIZE 50

/* struct that we didnt already have - used in the parser */
typedef struct
{
	union
	{
		t_quad			q_body;
		t_sphere		s_body;
	};
	t_solid_color	color;
	t_diffuse_light	difflight;
}			t_light;

/* for the parser */
/* camera inside t_mrt, ambient inside camera */
/*	All objects kept on the stack for faster execution
	(and easier memory management....) */
typedef struct
{
	t_light		lights[OBJECT_COUNT];
	t_sphere	spheres[SPHERES_COUNT];
	t_plane		planes[OBJECT_COUNT];
	t_cylinder_capped	cylinders[OBJECT_COUNT];
	t_quad		quads[OBJECT_COUNT];
	t_disk		disks[OBJECT_COUNT];
	t_triangle	triangles[OBJECT_COUNT];
	t_cone		cones[OBJECT_COUNT];
	t_box		boxes[OBJECT_COUNT];

	/* contains all shapes. lights as well */
	t_hittable *hit_list[OBJECT_COUNT * 8 + SPHERES_COUNT];
	int 		hit_idx;

	/* contains just lights */
	t_hittable *light_hit[OBJECT_COUNT];
	int			light_hit_idx;

	// data for the parser
	int			_file_fd;
	char		**_tokens;
}			t_objects;

typedef struct 	s_mrt
{
	void		*mlx;
	void		*win_ptr;
	mlx_image_t	*image;
	
	mlx_image_t *seconds_str;
    mlx_image_t *cores_str;

	double		mlx_time;
	bool needs_render;

	t_camera	cam;
	t_objects	objects;

	// hittable list
	t_hittablelist world;
	t_hittablelist lights;
	// render function
	void (*renderscene)(struct s_mrt *, const t_hittablelist *, const t_hittablelist *);

	// t_mouse_state mouse_state;
	// t_viewport	viewport;
	// t_pixel		pixel;
}				t_mrt;

void	parse_input(char *filename, t_mrt *data);

#endif
