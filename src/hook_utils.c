/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hook_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 15:20:15 by lbrusa            #+#    #+#             */
/*   Updated: 2024/10/14 14:58:08 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MLX42/MLX42.h"
#include "camera.h"
#include "debug.h"
#include "hook_utils.h"
#include "libft.h"
#include "minirt.h"
#include "utils.h"

/**
 * @Brief Handle the directions keys
 *
 * @param data The main data structure
 *
 * I will move the camera in the direction of the arrow keys
 * and update the camera orientation.
 * I will also print the new camera position and direction.
 */
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

/**
 * @brief Print the camera position and direction
 *
 * @param cam The camera object
 *
 * Utility function for debugging and orientation purposes.
 * Now commented out because my ft_printf doesnt take float arguments
 */
void	print_position(t_camera cam)
{
	printf("###############################################");
	printf("\ncamera center point = %f,%f,%f \ndirection %f,%f,%f\n",
		cam.orig.x,
	 	cam.orig.y, cam.orig.z, cam.dir.x, cam.dir.y, cam.dir.z);
	printf("horixontal field of view = %f", cam.hfov);
	printf("###############################################");
}

/**
 * @brief Handle the arrow keys
 *
 * @param data The main data structure
 * @param mlx The MLX42 library
 *
 * I will rotate the camera in the direction of the arrow keys
 * The camera position should not be affected by this.
 */
void	handle_arrow_keys(t_mrt *data, mlx_t *mlx)
{
	if (mlx_is_key_down(mlx, MLX_KEY_UP))
		rotate_camera_pitch(&(data->cam), degrees_to_radians(data->cam.hfov
				* ROTATION_DEG));
	else if (mlx_is_key_down(mlx, MLX_KEY_DOWN))
		rotate_camera_pitch(&(data->cam), degrees_to_radians(data->cam.hfov 
				* -ROTATION_DEG));
	else if (mlx_is_key_down(mlx, MLX_KEY_LEFT))
		rotate_camera_yaw(&(data->cam), degrees_to_radians(data->cam.hfov
				* ROTATION_DEG));
	else if (mlx_is_key_down(mlx, MLX_KEY_RIGHT))
		rotate_camera_yaw(&(data->cam), degrees_to_radians(data->cam.hfov 
				* -ROTATION_DEG));
	else
		return ;
	print_position(data->cam);
	data->needs_render = true;
	data->mlx_time = mlx_get_time();
}

/**
 * @brief Handle the reset key
 *
 * @param data The main data structure
 * @param mlx The MLX42 library
 *
 * With some scenes the movements are slow.
 * Sometimes I want to go quickly back to the original position.
 */
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

/**
 * @brief Handle the camera movements
 *
 * @param data The main data structure
 * @param mlx The MLX42 library
 *
 * I will move the camera forward or backward or to the left or right.
 */
void	handle_direction_key(t_mrt *data, mlx_t *mlx)
{
	if (mlx_is_key_down(mlx, MLX_KEY_W))
		move_camera_up(&(data->cam), data->cam.img_height / PIX_AMOUNT);
	else if (mlx_is_key_down(mlx, MLX_KEY_S))
		move_camera_up(&(data->cam), -data->cam.img_height / PIX_AMOUNT);
	else if (mlx_is_key_down(mlx, MLX_KEY_A))
		move_camera_right(&(data->cam), -data->cam.img_width / PIX_AMOUNT);
	else if (mlx_is_key_down(mlx, MLX_KEY_D))
		move_camera_right(&(data->cam), data->cam.img_width / PIX_AMOUNT);
	else
		return ;
	print_position(data->cam);
	data->needs_render = true;
	data->mlx_time = mlx_get_time();
}
