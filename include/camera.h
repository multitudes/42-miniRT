/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/22 16:37:03 by lbrusa            #+#    #+#             */
/*   Updated: 2024/09/22 14:33:31 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CAMERA_H
# define CAMERA_H

#include "minirt.h"
#include "ray.h"
#include "vec3.h"
#include "hittable_list.h"
#include "ambient.h"
#include <pthread.h>

#define ASPECT_RATIO (double)16.0/16.0
#define IMAGE_WIDTH 200
#define CORES 16
#define BONUS 1

typedef struct s_mrt t_mrt;

typedef struct	s_thread_data 
{
	t_mrt *data;
	int thread_id;
	const t_hittablelist *world;
	const t_hittablelist *lights;
	int starty;
	int endy;
} 				t_thread_data;

typedef struct	s_camera
{
	// considered "public" 
    t_point3 	center;         // Camera center
	t_vec3		direction;
	double 		aspect_ratio;
	int 		image_width;
	int    		image_height;   // Rendered image height
	t_vec3   	u, v, w;              // Camera frame basis vectors
	double		hfov;			   // hotizontal Field of view

	int 		samples_per_pixel;
	int			max_depth;		   // Maximum number of ray bounces into scene

	// t_point3 	lookfrom;   		// Point camera is looking from
    // t_point3 	lookat;  			// Point camera is looking at
    t_vec3   	vup;     			// Camera-relative "up" direction
	
	// considered private
    t_point3	pixel00_loc;    // Location of pixel 0, 0
    t_vec3		pixel_delta_u;  // Offset to pixel to the right
    t_vec3		pixel_delta_v;  // Offset to pixel below

	t_ambient  ambient_light;
	pthread_t threads[CORES];
	t_thread_data thread_data[CORES];
	void		(*print)(const void* self);
	
} 				t_camera;

void			init_cam(t_camera *cam, t_point3 center, t_vec3 direction, double hfov);
void    		render(t_mrt *data, const t_hittablelist* world, const t_hittablelist* lights);
t_color			ray_color(t_camera *cam, t_ray *r, int depth, const t_hittablelist *world, const t_hittablelist *lights);
void 			write_color(t_mrt *data, int x, int y, t_color colorvector);
void			print_camera(const void *self);
unsigned int    color_gamma_corrected(t_color color);
void 			update_cam(t_camera *cam, int new_width, int new_height);
// t_ray		get_ray(t_camera *c, int u, int v);
// t_vec3		sample_square();


#endif
