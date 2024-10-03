/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/22 16:37:03 by lbrusa            #+#    #+#             */
/*   Updated: 2024/10/03 17:35:45 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CAMERA_H
# define CAMERA_H

# include "ambient.h"
# include "hittable_list.h"
# include "ray.h"
# include "utils.h"
# include "vec3.h"
# include <pthread.h>

# define ASPECT_RATIO 1
# define IMAGE_WIDTH 400
# define BONUS 1

typedef struct s_mrt		t_mrt;

typedef struct s_thread_data
{
	t_mrt					*data;
	int						thread_id;
	const t_hittablelist	*world;
	const t_hittablelist	*lights;
	int						starty;
	int						endy;
}							t_thread_data;

typedef struct s_camera
{
	t_point3		orig;
	t_vec3			dir;
	double			aspect_ratio;
	int				img_width;
	int				img_height;
	t_vec3			u;
	t_vec3			v;
	t_vec3			w;
	t_vec3			viewport_u;
	t_vec3			viewport_v;
	double			hfov;
	int				samples_per_pixel;
	int				max_depth;
	t_vec3			vup;
	t_point3		pixel00_loc;
	t_vec3			pixel_delta_u;
	t_vec3			pixel_delta_v;
	t_point3		original_pos;
	t_vec3			original_dir;
	t_ambient		ambient;
	uint8_t			cores;
	pthread_t		threads[16];
	t_thread_data	thread_data[16];
	void			(*print)(const void *self);
}							t_camera;

void		init_cam(t_camera *cam, t_point3 center,
				t_vec3 direction, double hfov);
void		render(t_mrt *data, const t_hittablelist *world,
				const t_hittablelist *lights);
t_color		ray_color(t_camera *cam, t_ray *r, int depth,
				const t_hittablelist *world,
				const t_hittablelist *lights);
void		write_color(t_mrt *data, int x, int y,
				t_color colorvector);
void		print_camera(const void *self);
uint32_t 	color_gamma_corrected(t_color color);
void		update_cam_resize(t_camera *cam, int new_width,
				int new_height);
void		update_cam_orientation(t_camera *cam);

#endif
