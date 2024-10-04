/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hook_utils4.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 15:52:34 by lbrusa            #+#    #+#             */
/*   Updated: 2024/10/03 16:03:10 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MLX42/MLX42.h"
#include "camera.h"
#include "debug.h"
#include "hook_utils.h"
#include "libft.h"
#include "minirt.h"
#include "utils.h"

void	move_camera_forward(t_camera *cam, double distance)
{
	t_vec3	translation;

	translation = vec3multscalar(cam->w, distance);
	translate_camera(cam, translation);
}

// Function to calculate the new camera direction vector (pointing 
// toward the origin)
t_point3	calculate_direction(t_point3 camera_pos)
{
	t_point3	direction;

	direction = point3(-camera_pos.x, -camera_pos.y, -camera_pos.z);
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
