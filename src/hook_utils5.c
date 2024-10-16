/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hook_utils5.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 16:31:07 by lbrusa            #+#    #+#             */
/*   Updated: 2024/10/14 17:10:33 by lbrusa           ###   ########.fr       */
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
		write(2, "Space key pressed\n", 18);
	}
	else if (mlx_is_key_down(mlx, MLX_KEY_LEFT_SHIFT))
	{
		move_camera_forward(&(data->cam), data->cam.img_width / PIX_AMOUNT);
		write(2, "Left shift key pressed\n", 24);
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
			if (data->cam.cores == 1 && BONUS)
				data->cam.cores = sysconf(_SC_NPROCESSORS_ONLN);
			else
				data->cam.cores = 1;
			printf("cores: %d\n", data->cam.cores);
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
			write(2, "Rendering scene....\n", 21);
			render(data, &(data->world), &(data->lights));
		}
	}
}

/*
This is the callback of
mlx_resize_hook(params.mlx, &_resize_hook, (void*)&params);
The prototype of the function is given already.
I receive the new height and width from the system.
This works when resizing the window with the handles and also when going in
fullscreen mode for some reason, even if the full screen mode
is controlled differently in the background by the system.
*/
void	resize_hook(int new_width, int new_height, void *params)
{
	t_mrt	*data;

	data = ((t_mrt *)params);
	data->cam.img_width = new_width;
	data->cam.img_height = new_height;
	update_cam_resize(&data->cam, new_width, new_height);
	mlx_resize_image(data->image, (uint32_t)new_width, (uint32_t)new_height);
	data->needs_render = true;
}
