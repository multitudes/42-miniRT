/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hook_utils5.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 16:31:07 by lbrusa            #+#    #+#             */
/*   Updated: 2024/10/10 13:03:15 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MLX42/MLX42.h"
#include "camera.h"
#include "debug.h"
#include "hook_utils.h"
#include "libft.h"
#include "minirt.h"
#include "utils.h"

void	handle_hfow_key(t_mrt *data, mlx_t *mlx)
{
	if (mlx_is_key_down(mlx, MLX_KEY_F))
	{
		data->cam.hfov += 1;
		update_cam_orientation(&data->cam);
	}
	else if (mlx_is_key_down(mlx, MLX_KEY_G))
	{
		data->cam.hfov -= 1;
		update_cam_orientation(&data->cam);
	}
	else
		return ;
	data->needs_render = true;
	print_position(data->cam);
	data->mlx_time = mlx_get_time();
}

void	handle_zoom_key(t_mrt *data, mlx_t *mlx)
{
	if (mlx_is_key_down(mlx, MLX_KEY_SPACE))
	{
		move_camera_forward(&(data->cam), -data->cam.img_width / PIX_AMOUNT);
		debug("Space key pressed");
	}
	else if (mlx_is_key_down(mlx, MLX_KEY_LEFT_SHIFT))
	{
		move_camera_forward(&(data->cam), data->cam.img_width / PIX_AMOUNT);
		debug("Left shift key pressed");
	}
	else 
		return ;
	print_position(data->cam);
	data->needs_render = true;
	data->mlx_time = mlx_get_time();
}

/**
 * @brief Handle the key to switch between single and multithreading
 * 
 * @param data The main data structure
 * @param mlx The MLX42 library
 * 
 * Here it was important to handle the rebound correctly
 * so that the key is not triggered multiple times which 
 * would lead to problem by toggling the multithreading
 * I use a magic number of 0.1 to prevent the key from being
 * retriggered too fast. mlx_get_time returns the time in seconds
 */
void	handle_multithreading_key(t_mrt *data, mlx_t *mlx)
{
	if (mlx_is_key_down(mlx, MLX_KEY_F2))
	{
		if (mlx_get_time() - data->mlx_time > 0.1)
		{
			if (data->cam.cores == 1)
				data->cam.cores = CORES;
			else
				data->cam.cores = 1;
			debug("cores: %d\n", data->cam.cores);
			print_position(data->cam);
			data->needs_render = true;
			mlx_delete_image(data->mlx, data->cores_str);
			mlx_delete_image(data->mlx, data->seconds_str);
			data->mlx_time = mlx_get_time();
		}
	}
}

void	render_if_needed(t_mrt *data)
{
	if (data->needs_render)
	{
		if (mlx_get_time() - data->mlx_time > 0.05)
		{
			data->needs_render = false;
			debug("Rendering scene....");
			render(data, &(data->world), &(data->lights));
		}
	}
}
