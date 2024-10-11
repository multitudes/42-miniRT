/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/22 16:37:03 by lbrusa            #+#    #+#             */
/*   Updated: 2024/10/11 14:19:28 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CAMERA_H
# define CAMERA_H

# include "ambient.h"
# include "hittable_list.h"
# include "material.h"
# include "pdf.h"
# include "ray.h"
# include "utils.h"
# include "vec3.h"
# include <pthread.h>

# define ASPECT_RATIO 1
# define IMAGE_WIDTH 400

# ifdef DOBONUS
#  define BONUS 1
# else
#  define BONUS 0
# endif

typedef struct s_mrt		t_mrt;
typedef struct s_camera		t_camera;

typedef struct s_scene
{
	t_camera				*cam;
	const t_hittablelist	*world;
	const t_hittablelist	*lights;
}							t_scene;

typedef struct s_thread_data
{
	t_mrt					*data;
	int						thread_id;
	t_scene					scene;
	int						starty;
	int						endy;
}							t_thread_data;

typedef struct s_camera
{
	t_point3				orig;
	t_vec3					dir;
	double					aspect_ratio;
	int						img_width;
	int						img_height;
	t_vec3					u;
	t_vec3					v;
	t_vec3					w;
	t_vec3					viewport_u;
	t_vec3					viewport_v;
	double					hfov;
	int						samples_per_pixel;
	int						max_depth;
	t_vec3					vup;
	t_point3				pixel00_loc;
	t_vec3					pixel_delta_u;
	t_vec3					pixel_delta_v;
	t_point3				original_pos;
	t_vec3					original_dir;
	t_ambient				ambient;
	uint8_t					cores;
	pthread_t				threads[CORES];
	t_thread_data			thread_data[CORES];
	void					(*print)(const void *self);
}							t_camera;

typedef struct s_rcparams
{
	t_hit_record			rec;
	t_scatter_record		srec;
	t_hittable_pdf			light_pdf;
	t_pdf					*recorded_pdf;
	t_color					color_from_emission;
	t_ray					scattered;
	double					pdf_value;
	double					scattering_pdf;
	t_mixture_pdf			mix_pdf;
	t_color					sample_color;
}							t_rcparams;

void						init_cam(t_camera *cam, t_point3 center,
								t_vec3 direction, double hfov);
void						render(t_mrt *data, const t_hittablelist *world,
								const t_hittablelist *lights);
t_color						ray_color(t_scene scene, t_ray *r, int depth);
t_ray						get_ray(t_camera cam, int i, int j);
void						write_color(t_mrt *data, int x, int y,
								t_color colorvector);
void						print_camera(const void *self);
uint32_t					color_gamma_corrected(t_color color);
void						update_cam_resize(t_camera *cam, int new_width,
								int new_height);
void						update_cam_orientation(t_camera *cam);
t_color						metal_color_mix(t_scene scene, t_hit_record rec,
								t_scatter_record srec, int depth);
void						init_rcparams(t_rcparams *params);
t_color						get_color_from_scatter(t_rcparams params,
								t_scene scene, t_ray *r, int depth);
void						update_time(double start_time, t_mrt *data);

#endif
