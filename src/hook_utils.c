/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hook_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 15:20:15 by lbrusa            #+#    #+#             */
/*   Updated: 2024/10/07 13:53:29 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MLX42/MLX42.h"
#include "camera.h"
#include "debug.h"
#include "hook_utils.h"
#include "libft.h"
#include "minirt.h"
#include "utils.h"

void 	handle_arrow_keys(t_mrt *data, mlx_t *mlx)
{
	if (mlx_is_key_down(mlx, MLX_KEY_UP))
	{
		debug("Arrow Up  - pitch up- pressed\n");
		rotate_camera_pitch(&(data->cam), degrees_to_radians(data->cam.hfov
				* ROTATION_DEG));
		debug("camera center point = %f %f %f and direction %f %f %f\n",
			data->cam.orig.x, data->cam.orig.y, data->cam.orig.z,
			data->cam.dir.x, data->cam.dir.y,
			data->cam.dir.z);
		data->needs_render = true;
		data->mlx_time = mlx_get_time();
	}
	if (mlx_is_key_down(mlx, MLX_KEY_DOWN))
	{
		debug("pitch Arrow down pressed\n");
		rotate_camera_pitch(&(data->cam), degrees_to_radians(data->cam.hfov \
		* -ROTATION_DEG));
		debug("camera center point = %f %f %f and direction %f %f %f\n",
			data->cam.orig.x, data->cam.orig.y, data->cam.orig.z,
			data->cam.dir.x, data->cam.dir.y,
			data->cam.dir.z);
		data->needs_render = true;
		data->mlx_time = mlx_get_time();
	}
	if (mlx_is_key_down(mlx, MLX_KEY_LEFT))
	{
		debug("yaw Arrow left pressed\n");
		rotate_camera_yaw(&(data->cam), degrees_to_radians(data->cam.hfov
				* ROTATION_DEG));
		debug("camera center point = %f %f %f and direction %f %f %f\n",
			data->cam.orig.x, data->cam.orig.y, data->cam.orig.z,
			data->cam.dir.x, data->cam.dir.y,
			data->cam.dir.z);
		data->needs_render = true;
		data->mlx_time = mlx_get_time();
	}
	if (mlx_is_key_down(mlx, MLX_KEY_RIGHT))
	{
		debug("yaw + Arrow right pressed\n");
		rotate_camera_yaw(&(data->cam), degrees_to_radians(data->cam.hfov \
		* -ROTATION_DEG));
		debug("camera center point = %f %f %f and direction %f %f %f\n",
			data->cam.orig.x, data->cam.orig.y, data->cam.orig.z,
			data->cam.dir.x, data->cam.dir.y,
			data->cam.dir.z);
		data->needs_render = true;
		data->mlx_time = mlx_get_time();
	}
}

void	handle_reset_key(t_mrt *data, mlx_t *mlx)
{
	if (mlx_is_key_down(mlx, MLX_KEY_F1))
	{
		debug("F1 reset pressed\n");
		data->cam.orig = data->cam.original_pos;
		data->cam.dir = data->cam.original_dir;
		update_cam_orientation(&data->cam);
		data->needs_render = true;
		data->mlx_time = mlx_get_time();
	}
}

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
		move_camera_up(&(data->cam), data->cam.img_height / 20);
		debug("W UP key pressed");
		debug("camera center point = %f %f %f and direction %f %f %f\n",
			data->cam.orig.x, data->cam.orig.y, data->cam.orig.z,
			data->cam.dir.x, data->cam.dir.y,
			data->cam.dir.z);
		data->needs_render = true;
		data->mlx_time = mlx_get_time();
	}
	if (mlx_is_key_down(mlx, MLX_KEY_S))
	{
		move_camera_up(&(data->cam), -data->cam.img_height / 20);
		debug("S DOWN key pressed");
		debug("camera center point = %f %f %f and direction %f %f %f\n",
			data->cam.orig.x, data->cam.orig.y, data->cam.orig.z,
			data->cam.dir.x, data->cam.dir.y,
			data->cam.dir.z);
		data->needs_render = true;
		data->mlx_time = mlx_get_time();
	}
	if (mlx_is_key_down(mlx, MLX_KEY_A))
	{
		move_camera_right(&(data->cam), -data->cam.img_width / 20);
		debug("A move left key pressed");
		debug("camera center point = %f %f %f and direction %f %f %f\n",
			data->cam.orig.x, data->cam.orig.y, data->cam.orig.z,
			data->cam.dir.x, data->cam.dir.y,
			data->cam.dir.z);
		data->needs_render = true;
		data->mlx_time = mlx_get_time();
	}
	if (mlx_is_key_down(mlx, MLX_KEY_D))
	{
		move_camera_right(&(data->cam), data->cam.img_width / 20);
		debug("D more right key pressed");
		debug("camera center point = %f %f %f and direction %f %f %f\n",
			data->cam.orig.x, data->cam.orig.y, data->cam.orig.z,
			data->cam.dir.x, data->cam.dir.y,
			data->cam.dir.z);
		data->needs_render = true;
		data->mlx_time = mlx_get_time();
	}
	if (mlx_is_key_down(mlx, MLX_KEY_SPACE))
	{
		move_camera_forward(&(data->cam), -data->cam.img_width / 20);
		debug("Space key pressed");
		debug("camera center point = %f %f %f and direction %f %f %f\n",
			data->cam.orig.x, data->cam.orig.y, data->cam.orig.z,
			data->cam.dir.x, data->cam.dir.y,
			data->cam.dir.z);
		data->needs_render = true;
		data->mlx_time = mlx_get_time();
	}
	if (mlx_is_key_down(mlx, MLX_KEY_LEFT_SHIFT))
	{
		move_camera_forward(&(data->cam), data->cam.img_width / 20);
		debug("Left shift key pressed");
		debug("camera center point = %f %f %f and direction %f %f %f\n",
			data->cam.orig.x, data->cam.orig.y, data->cam.orig.z,
			data->cam.dir.x, data->cam.dir.y,
			data->cam.dir.z);
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
				data->cam.cores = CORES;
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
