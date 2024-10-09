/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hook_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 15:20:15 by lbrusa            #+#    #+#             */
/*   Updated: 2024/10/09 16:13:31 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MLX42/MLX42.h"
#include "camera.h"
#include "debug.h"
#include "hook_utils.h"
#include "libft.h"
#include "minirt.h"
#include "utils.h"

#include "MLX42/MLX42.h"
#include "camera.h"
#include "debug.h"
#include "hook_utils.h"
#include "libft.h"
#include "minirt.h"
#include "utils.h"

void	hook(void *param)
{
	mlx_t	*mlx;
	t_mrt	*data;

	data = (t_mrt *)param;
	mlx = data->mlx;
	if (mlx_is_key_down(mlx, MLX_KEY_ESCAPE))
		exit_gracefully(mlx);
	handle_arrow_keys(data, mlx);
	handle_reset_key(data, mlx);
	handle_hfow_key(data, mlx);
	handle_direction_key(data, mlx);
	handle_zoom_key(data, mlx);
	handle_multithreading_key(data, mlx);
	render_if_needed(data);
}

void	print_position(t_camera cam)
{
	debug("camera center point = %f %f %f and direction %f %f %f\n", cam.orig.x,
		cam.orig.y, cam.orig.z, cam.dir.x, cam.dir.y, cam.dir.z);
}

void	handle_arrow_keys(t_mrt *data, mlx_t *mlx)
{
	if (mlx_is_key_down(mlx, MLX_KEY_UP))
		rotate_camera_pitch(&(data->cam), degrees_to_radians(data->cam.hfov
				* ROTATION_DEG));
	else if (mlx_is_key_down(mlx, MLX_KEY_DOWN))
		rotate_camera_pitch(&(data->cam), degrees_to_radians(data->cam.hfov * \
		-ROTATION_DEG));
	else if (mlx_is_key_down(mlx, MLX_KEY_LEFT))
		rotate_camera_yaw(&(data->cam), degrees_to_radians(data->cam.hfov
				* ROTATION_DEG));
	else if (mlx_is_key_down(mlx, MLX_KEY_RIGHT))
		rotate_camera_yaw(&(data->cam), degrees_to_radians(data->cam.hfov * \
		-ROTATION_DEG));
	else
		return ;
	print_position(data->cam);
	data->needs_render = true;
	data->mlx_time = mlx_get_time();
}

void	handle_reset_key(t_mrt *data, mlx_t *mlx)
{
	if (mlx_is_key_down(mlx, MLX_KEY_F1))
	{
		data->cam.orig = data->cam.original_pos;
		data->cam.dir = data->cam.original_dir;
		update_cam_orientation(&data->cam);
		data->needs_render = true;
		data->mlx_time = mlx_get_time();
		print_position(data->cam);
	}
}

void	handle_direction_key(t_mrt *data, mlx_t *mlx)
{
	if (mlx_is_key_down(mlx, MLX_KEY_W))
		move_camera_up(&(data->cam), data->cam.img_height / 40);
	else if (mlx_is_key_down(mlx, MLX_KEY_S))
		move_camera_up(&(data->cam), -data->cam.img_height / 40);
	else if (mlx_is_key_down(mlx, MLX_KEY_A))
		move_camera_right(&(data->cam), -data->cam.img_width / 40);
	else if (mlx_is_key_down(mlx, MLX_KEY_D))
		move_camera_right(&(data->cam), data->cam.img_width / 40);
	else
		return ;
	print_position(data->cam);
	data->needs_render = true;
	data->mlx_time = mlx_get_time();
}
