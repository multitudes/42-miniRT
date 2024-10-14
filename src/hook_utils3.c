/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hook_utils3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 15:52:07 by lbrusa            #+#    #+#             */
/*   Updated: 2024/10/14 16:03:01 by lbrusa           ###   ########.fr       */
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
	print_position(*cam);
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
	print_position(*cam);
	update_cam_orientation(cam);
}

void	exit_gracefully(mlx_t *mlx)
{
	write(1, "byebye!\n", 8);
	mlx_close_window(mlx);
	mlx_terminate(mlx);
	exit(EXIT_SUCCESS);
}

void	translate_camera(t_camera *cam, t_vec3 translation)
{
	cam->orig = vec3add(cam->orig, translation);
	print_position(*cam);
	update_cam_resize(cam, cam->img_width, cam->img_height);
}
