/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hook_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 15:20:15 by lbrusa            #+#    #+#             */
/*   Updated: 2024/10/03 15:56:57 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	if (mlx_is_key_down(mlx, MLX_KEY_UP))
	{
		debug("Arrow Up  - pitch up- pressed\n");
		rotate_camera_pitch(&(data->cam), degrees_to_radians(data->cam.hfov
				* ROTATION_DEG));
		debug("camera center point = %f %f %f and direction %f %f %f\n",
			data->cam.center.x, data->cam.center.y, data->cam.center.z,
			data->cam.direction.x, data->cam.direction.y,
			data->cam.direction.z);
		data->needs_render = true;
		data->mlx_time = mlx_get_time();
	}
	if (mlx_is_key_down(mlx, MLX_KEY_DOWN))
	{
		debug("pitch Arrow down pressed\n");
		rotate_camera_pitch(&(data->cam), degrees_to_radians(data->cam.hfov * -ROTATION_DEG));
		debug("camera center point = %f %f %f and direction %f %f %f\n",
			data->cam.center.x, data->cam.center.y, data->cam.center.z,
			data->cam.direction.x, data->cam.direction.y,
			data->cam.direction.z);
		data->needs_render = true;
		data->mlx_time = mlx_get_time();
	}
	if (mlx_is_key_down(mlx, MLX_KEY_LEFT))
	{
		debug("yaw Arrow left pressed\n");
		rotate_camera_yaw(&(data->cam), degrees_to_radians(data->cam.hfov
				* ROTATION_DEG));
		debug("camera center point = %f %f %f and direction %f %f %f\n",
			data->cam.center.x, data->cam.center.y, data->cam.center.z,
			data->cam.direction.x, data->cam.direction.y,
			data->cam.direction.z);
		data->needs_render = true;
		data->mlx_time = mlx_get_time();
	}
	if (mlx_is_key_down(mlx, MLX_KEY_RIGHT))
	{
		debug("yaw + Arrow right pressed\n");
		rotate_camera_yaw(&(data->cam), degrees_to_radians(data->cam.hfov *
				-ROTATION_DEG));
		debug("camera center point = %f %f %f and direction %f %f %f\n",
			data->cam.center.x, data->cam.center.y, data->cam.center.z,
			data->cam.direction.x, data->cam.direction.y,
			data->cam.direction.z);
		data->needs_render = true;
		data->mlx_time = mlx_get_time();
	}
	if (mlx_is_key_down(mlx, MLX_KEY_F1))
	{
		// reset camera to original position
		debug("F1 reset pressed\n");
		data->cam.center = data->cam.original_pos;
		data->cam.direction = data->cam.original_dir;
		update_cam_orientation(&data->cam);
		data->needs_render = true;
		data->mlx_time = mlx_get_time();
	}
	if (mlx_is_key_down(mlx, MLX_KEY_F))
	{
		data->cam.hfov += 1;
		update_cam_orientation(&data->cam);
		data->needs_render = true;
		debug("F hfov+ key pressed %f\n", data->cam.hfov);
		data->mlx_time = mlx_get_time();
	}
	if (mlx_is_key_down(mlx, MLX_KEY_G))
	{
		data->cam.hfov -= 1;
		update_cam_orientation(&data->cam);
		data->needs_render = true;
		debug("G hfov- key pressed %f\n", data->cam.hfov);
		data->mlx_time = mlx_get_time();
	}
	if (mlx_is_key_down(mlx, MLX_KEY_W))
	{
		move_camera_up(&(data->cam), data->cam.image_height / 20);
		debug("W UP key pressed");
		debug("camera center point = %f %f %f and direction %f %f %f\n",
			data->cam.center.x, data->cam.center.y, data->cam.center.z,
			data->cam.direction.x, data->cam.direction.y,
			data->cam.direction.z);
		data->needs_render = true;
		data->mlx_time = mlx_get_time();
	}
	if (mlx_is_key_down(mlx, MLX_KEY_S))
	{
		move_camera_up(&(data->cam), -data->cam.image_height / 20);
		debug("S DOWN key pressed");
		debug("camera center point = %f %f %f and direction %f %f %f\n",
			data->cam.center.x, data->cam.center.y, data->cam.center.z,
			data->cam.direction.x, data->cam.direction.y,
			data->cam.direction.z);
		data->needs_render = true;
		data->mlx_time = mlx_get_time();
	}
	if (mlx_is_key_down(mlx, MLX_KEY_A))
	{
		move_camera_right(&(data->cam), -data->cam.image_width / 20);
		debug("A move left key pressed");
		debug("camera center point = %f %f %f and direction %f %f %f\n",
			data->cam.center.x, data->cam.center.y, data->cam.center.z,
			data->cam.direction.x, data->cam.direction.y,
			data->cam.direction.z);
		data->needs_render = true;
		data->mlx_time = mlx_get_time();
	}
	if (mlx_is_key_down(mlx, MLX_KEY_D))
	{
		move_camera_right(&(data->cam), data->cam.image_width / 20);
		debug("D more right key pressed");
		debug("camera center point = %f %f %f and direction %f %f %f\n",
			data->cam.center.x, data->cam.center.y, data->cam.center.z,
			data->cam.direction.x, data->cam.direction.y,
			data->cam.direction.z);
		data->needs_render = true;
		data->mlx_time = mlx_get_time();
	}
	if (mlx_is_key_down(mlx, MLX_KEY_SPACE))
	{
		move_camera_forward(&(data->cam), -data->cam.image_width / 20);
		debug("S key pressed");
		debug("camera center point = %f %f %f and direction %f %f %f\n",
			data->cam.center.x, data->cam.center.y, data->cam.center.z,
			data->cam.direction.x, data->cam.direction.y,
			data->cam.direction.z);
		data->needs_render = true;
		data->mlx_time = mlx_get_time();
	}
	if (mlx_is_key_down(mlx, MLX_KEY_LEFT_SHIFT))
	{
		move_camera_forward(&(data->cam), data->cam.image_width / 10);
		debug("Left shift key pressed");
		debug("camera center point = %f %f %f and direction %f %f %f\n",
			data->cam.center.x, data->cam.center.y, data->cam.center.z,
			data->cam.direction.x, data->cam.direction.y,
			data->cam.direction.z);
		data->needs_render = true;
		data->mlx_time = mlx_get_time();
	}
	if (mlx_is_key_down(mlx, MLX_KEY_F2))
	{
		if (mlx_get_time() - data->mlx_time > 0.1)
		{
			debug("mlx_get_time() - data->mlx_time %f\n", mlx_get_time()
				- data->mlx_time);
			debug("F2 cores toggle pressed\n");
			if (data->cam.cores == 1)
				data->cam.cores = 16;
			else
				data->cam.cores = 1;
			debug("cores: %d\n", data->cam.cores);
			data->needs_render = true;
			mlx_delete_image(data->mlx, data->cores_str);
			mlx_delete_image(data->mlx, data->seconds_str);
			data->mlx_time = mlx_get_time();
		}
	}
	if (data->needs_render)
	{
		if (mlx_get_time() - data->mlx_time > 0.05)
		{
			data->needs_render = false;
			render(data, &(data->world), &(data->lights));
		}
	}
}
