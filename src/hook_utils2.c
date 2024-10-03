/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hook_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 15:50:50 by lbrusa            #+#    #+#             */
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
