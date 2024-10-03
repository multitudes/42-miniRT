/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hook_utils3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 15:52:07 by lbrusa            #+#    #+#             */
/*   Updated: 2024/10/03 17:34:11 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MLX42/MLX42.h"
#include "camera.h"
#include "debug.h"
#include "hook_utils.h"
#include "libft.h"
#include "minirt.h"
#include "utils.h"

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
	new_y = camera.y;
	new_x = camera.x * cos(angle_radians) - camera.z * sin(angle_radians);
	new_z = camera.x * sin(angle_radians) + camera.z * cos(angle_radians);
	return (point3(new_x, new_y, new_z));
}

void	translate_camera(t_camera *cam, t_vec3 translation)
{
	cam->orig = vec3add(cam->orig, translation);
	update_cam_resize(cam, cam->img_width, cam->img_height);
}
