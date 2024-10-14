/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/22 16:34:23 by lbrusa            #+#    #+#             */
/*   Updated: 2024/10/14 13:59:21 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# include "box.h"
# include "camera.h"
# include "cone.h"
# include "cylinder.h"
# include "disk.h"
# include "hittable.h"
# include "material.h"
# include "plane.h"
# include "quad.h"
# include "sphere.h"
# include "texture.h"
# include "triangle.h"
# include "vec3.h"
# include <MLX42/MLX42.h>
# include <limits.h>

# define TRUE 1
# define FALSE 0
# define OBJECT_COUNT 20
# define SPHERES_COUNT 1000
# define CYLINDER_COUNT 100
# define CHECKER_SIZE 1
# define TRIANGLE_COUNT 100
# define DISK_COUNT 250

# ifndef PATH_MAX
#  define PATH_MAX 256
# endif

/**
 * @brief A light source
 *
 * it can be a quad or a sphere
 * this struct is used in the parser
 */
typedef struct s_light
{
	union
	{
		t_quad				q_body;
		t_sphere			s_body;
	};
	t_solid_color			color;
	union
	{
		t_diffuse_light		difflight;
		t_empty_material	no_material;
	};
}						t_light;

/**
 * @brief A struct to hold all objects created in the scene
 * All objects kept on the stack for faster execution
 * (and easier memory management....)
 */
typedef struct s_objects
{
	t_light				lights[OBJECT_COUNT];
	t_light				empty_lights[OBJECT_COUNT];
	t_sphere			spheres[SPHERES_COUNT];
	t_plane				planes[OBJECT_COUNT];
	t_cylinder_capped	cylinders[CYLINDER_COUNT];
	t_cylinder			cylinders_u[CYLINDER_COUNT];
	t_quad				quads[OBJECT_COUNT];
	t_disk				disks[DISK_COUNT];
	t_triangle			triangles[TRIANGLE_COUNT];
	t_cone				cones[OBJECT_COUNT];
	t_cone_uncap		cones_u[OBJECT_COUNT];
	t_box				boxes[OBJECT_COUNT];
	t_hittable			*hit_list[OBJECT_COUNT * 8 + SPHERES_COUNT + 
		(2 * CYLINDER_COUNT) + DISK_COUNT + TRIANGLE_COUNT];
	int					hit_idx;
	t_hittable			*light_hit[OBJECT_COUNT + 1];
	int					light_idx;
	int					_file_fd;
	char				**_tokens;
}						t_objects;

typedef struct s_mrt
{
	void				*mlx;
	void				*win_ptr;
	char				win_title[PATH_MAX];
	mlx_image_t			*image;
	mlx_image_t			*filt_img;
	mlx_image_t			*seconds_str;
	mlx_image_t			*cores_str;
	double				mlx_time;
	bool				needs_render;
	t_camera			cam;
	t_objects			objects;
	t_hittablelist		world;
	t_hittablelist		lights;
	void				(*renderscene)(struct s_mrt *, const t_hittablelist *, \
										const t_hittablelist *);
}						t_mrt;

void					parse_input(char *filename, t_mrt *data);

#endif
