/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 15:20:15 by lbrusa            #+#    #+#             */
/*   Updated: 2024/09/30 09:59:33 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MLX42/MLX42.h"
#include "camera.h"
#include "debug.h"
#include "hooks_utils.h"
#include "libft.h"
#include "minirt.h"
#include "utils.h"

void	hook(void *param)
{
	mlx_t	*mlx;
	t_mrt	*data;

	// mlx start time
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
		rotate_camera_pitch(&(data->cam), degrees_to_radians(data->cam.hfov *
				-ROTATION_DEG));
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
			data->cam.cores = data->cam.cores == 1 ? 16 : 1;
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

// Function to rotate a vector using a 3x3 rotation matrix
t_vec3	rotate_vector(t_vec3 v, double matrix[3][3])
{
	t_vec3	result;

	result.x = matrix[0][0] * v.x + matrix[0][1] * v.y + matrix[0][2] * v.z;
	result.y = matrix[1][0] * v.x + matrix[1][1] * v.y + matrix[1][2] * v.z;
	result.z = matrix[2][0] * v.x + matrix[2][1] * v.y + matrix[2][2] * v.z;
	return (result);
}

// Function to create a yaw rotation matrix (around the y-axis)
void	create_yaw_rotation_matrix(double angle, double matrix[3][3])
{
	double	cos_angle;
	double	sin_angle;

	cos_angle = cos(angle);
	sin_angle = sin(angle);
	matrix[0][0] = cos_angle;
	matrix[0][1] = 0;
	matrix[0][2] = sin_angle;
	matrix[1][0] = 0;
	matrix[1][1] = 1;
	matrix[1][2] = 0;
	matrix[2][0] = -sin_angle;
	matrix[2][1] = 0;
	matrix[2][2] = cos_angle;
}

// Function to create a pitch rotation matrix (around the x-axis)
void	create_pitch_rotation_matrix(double angle, double matrix[3][3])
{
	double	cos_angle;
	double	sin_angle;

	cos_angle = cos(angle);
	sin_angle = sin(angle);
	matrix[0][0] = 1;
	matrix[0][1] = 0;
	matrix[0][2] = 0;
	matrix[1][0] = 0;
	matrix[1][1] = cos_angle;
	matrix[1][2] = -sin_angle;
	matrix[2][0] = 0;
	matrix[2][1] = sin_angle;
	matrix[2][2] = cos_angle;
}

// Function to create a roll rotation matrix (around the z-axis)
void	create_roll_rotation_matrix(double angle, double matrix[3][3])
{
	double	cos_angle;
	double	sin_angle;

	cos_angle = cos(angle);
	sin_angle = sin(angle);
	matrix[0][0] = cos_angle;
	matrix[0][1] = -sin_angle;
	matrix[0][2] = 0;
	matrix[1][0] = sin_angle;
	matrix[1][1] = cos_angle;
	matrix[1][2] = 0;
	matrix[2][0] = 0;
	matrix[2][1] = 0;
	matrix[2][2] = 1;
}

// Function to rotate the camera around the y-axis (yaw)
void	rotate_camera_yaw(t_camera *cam, double angle)
{
	double	rotation_matrix[3][3];

	create_yaw_rotation_matrix(angle, rotation_matrix);
	cam->u = rotate_vector(cam->u, rotation_matrix);
	cam->v = rotate_vector(cam->v, rotation_matrix);
	cam->w = rotate_vector(cam->w, rotation_matrix);
	update_cam_orientation(cam);
}

// Function to rotate the camera around the x-axis (pitch)
void	rotate_camera_pitch(t_camera *cam, double angle)
{
	double	rotation_matrix[3][3];

	create_pitch_rotation_matrix(angle, rotation_matrix);
	cam->u = rotate_vector(cam->u, rotation_matrix);
	cam->v = rotate_vector(cam->v, rotation_matrix);
	cam->w = rotate_vector(cam->w, rotation_matrix);
	update_cam_orientation(cam);
}

// Function to rotate the camera around the z-axis (roll)
void	rotate_camera_roll(t_camera *cam, double angle)
{
	double	rotation_matrix[3][3];

	create_roll_rotation_matrix(angle, rotation_matrix);
	cam->u = rotate_vector(cam->u, rotation_matrix);
	cam->v = rotate_vector(cam->v, rotation_matrix);
	cam->w = rotate_vector(cam->w, rotation_matrix);
	update_cam_orientation(cam);
}

void	exit_gracefully(mlx_t *mlx)
{
	ft_printf("byebye!\n");
	mlx_close_window(mlx);
	mlx_terminate(mlx);
	exit(EXIT_SUCCESS);
}

// keep y the same rotate around the y axis
t_point3	rotate_camera(t_point3 camera, double angle_degrees)
{
	double	angle_radians;
	double	new_y;
	double	new_x;
	double	new_z;

	angle_radians = degrees_to_radians(angle_degrees);
	// Keep the y coordinate the same
	new_y = camera.y;
	// Calculate the new x and z using the rotation matrix
	new_x = camera.x * cos(angle_radians) - camera.z * sin(angle_radians);
	new_z = camera.x * sin(angle_radians) + camera.z * cos(angle_radians);
	// Return the new camera position
	return (point3(new_x, new_y, new_z));
}

void	translate_camera(t_camera *cam, t_vec3 translation)
{
	cam->center = vec3add(cam->center, translation);
	update_cam_resize(cam, cam->image_width, cam->image_height);
}

void	move_camera_forward(t_camera *cam, double distance)
{
	t_vec3	translation;

	translation = vec3multscalar(cam->w, distance);
	translate_camera(cam, translation);
}

// Function to calculate the new camera direction vector (pointing toward the origin)
t_point3	calculate_direction(t_point3 camera_pos)
{
	t_point3	direction;

	// Direction vector from the camera to the origin (0, 0, 0)
	direction = point3(-camera_pos.x, -camera_pos.y, -camera_pos.z);
	// Normalize the direction vector
	return (unit_vector(direction));
}

void	move_camera_right(t_camera *cam, double distance)
{
	t_vec3	translation;

	translation = vec3multscalar(cam->u, distance);
	translate_camera(cam, translation);
}

void	move_camera_up(t_camera *cam, double distance)
{
	t_vec3	translation;

	translation = vec3multscalar(cam->v, distance);
	translate_camera(cam, translation);
}
