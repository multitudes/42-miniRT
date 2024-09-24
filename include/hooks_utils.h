/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks_utils.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 15:21:18 by lbrusa            #+#    #+#             */
/*   Updated: 2024/09/24 15:37:47 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HOOKS_UTILS_H
# define HOOKS_UTILS_H

#include "camera.h"
#include "MLX42/MLX42.h"
#define ROTATION_DEG 0.005

void    exit_gracefully(mlx_t *mlx);
void    rotate_camera_yaw(t_camera *cam, double angle);
void    rotate_camera_pitch(t_camera *cam, double angle);
void    rotate_camera_roll(t_camera *cam, double angle);
void    move_camera_forward(t_camera *cam, double distance);
void    move_camera_right(t_camera *cam, double distance);
void    move_camera_up(t_camera *cam, double distance);
void    _resize_hook(int new_width, int new_height, void *params);
void	hook(void *param);
void    key_callback(mlx_key_data_t keydata, void* param);

#endif