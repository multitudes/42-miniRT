/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hook_utils.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 15:21:18 by lbrusa            #+#    #+#             */
/*   Updated: 2024/09/30 09:39:01 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HOOK_UTILS_H
# define HOOK_UTILS_H

# include "MLX42/MLX42.h"
# include "camera.h"
# define ROTATION_DEG 0.005
# define PIX_AMOUNT 100

void		exit_gracefully(mlx_t *mlx);
void		rotate_camera_yaw(t_camera *cam, double angle);
void		rotate_camera_pitch(t_camera *cam, double angle);
void		rotate_camera_roll(t_camera *cam, double angle);
void		move_camera_forward(t_camera *cam, double distance);
void		move_camera_right(t_camera *cam, double distance);
void		move_camera_up(t_camera *cam, double distance);
void		_resize_hook(int new_width, int new_height, void *params);
void		hook(void *param);
t_point3	calculate_direction(t_point3 camera_pos);
void		translate_camera(t_camera *cam, t_vec3 translation);
t_vec3		rotate_vector(t_vec3 v, double matrix[3][3]);
void		create_yaw_rotation_matrix(double angle, double matrix[3][3]);
void		create_pitch_rotation_matrix(double angle, double matrix[3][3]);
void		create_roll_rotation_matrix(double angle, double matrix[3][3]);
void		rotate_camera_yaw(t_camera *cam, double angle);
void		handle_arrow_keys(t_mrt *data, mlx_t *mlx);
void		handle_reset_key(t_mrt *data, mlx_t *mlx);
void		handle_hfow_key(t_mrt *data, mlx_t *mlx);
void		handle_direction_key(t_mrt *data, mlx_t *mlx);
void		handle_zoom_key(t_mrt *data, mlx_t *mlx);
void		handle_multithreading_key(t_mrt *data, mlx_t *mlx);
void		render_if_needed(t_mrt *data);
void		print_position(t_camera cam);

#endif