/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 10:28:07 by lbrusa            #+#    #+#             */
/*   Updated: 2024/10/14 11:04:27 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "camera.h"
#include "debug.h"
#include "minirt.h"
#include "ray.h"
#include "utils.h"
#include "vec3.h"

/**
 * @brief initialize a camera object
 * 
 * @param cam the camera object
 * @param center the center of the camera
 * @param direction the direction the camera is facing
 * @param hfov the horizontal field of view
 * 
 */
void	init_cam(t_camera *cam, t_point3 center, t_vec3 direction, double hfov)
{
	cam->samples_per_pixel = 200;
	cam->max_depth = 200;
	if (direction.x == 0 && direction.z == 0)
		direction.z -= 0.1;
	cam->dir = unit_vector(direction);
	cam->original_dir = cam->dir;
	cam->original_pos = center;
	cam->aspect_ratio = ASPECT_RATIO;
	cam->img_width = IMAGE_WIDTH;
	cam->img_height = (int)(IMAGE_WIDTH / cam->aspect_ratio);
	if (cam->img_height < 1)
		cam->img_height = 1;
	cam->orig = center;
	cam->hfov = clamp(interval(1, 170), hfov);
	if (cam->ambient.ratio == 0)
		ambient(&cam->ambient, 0.2, (t_rgb){.r = 10, .g = 10, .b = 10});
	cam->w = unit_vector(vec3substr(cam->orig, vec3add(cam->orig, cam->dir)));
	cam->u = unit_vector(cross(cam->vup, cam->w));
	cam->v = cross(cam->w, cam->u);
	cam->vup = vec3(0, 1, 0);
	update_cam_resize(cam, cam->img_width, cam->img_height);
}

/**
 * @brief update a camera object when the orientation changes
 */
void	update_cam_orientation(t_camera *cam)
{
	t_point3	viewport_upper_left;
	t_point3	part;
	double		focal_length;
	double		viewport_width;
	double		viewport_height;

	if (cam->dir.x == 0 && cam->dir.z == 0)
		cam->dir.x -= 0.1;
	cam->dir = vec3negate(cam->w);
	focal_length = length(vec3substr(cam->orig, vec3add(cam->orig, cam->dir)));
	viewport_width = 2 * (tan((degrees_to_radians(cam->hfov)) / 2))
		* focal_length;
	viewport_height = viewport_width * ((double)cam->img_height
			/ cam->img_width);
	cam->viewport_u = vec3multscalar(cam->u, viewport_width);
	cam->viewport_v = vec3multscalar(vec3negate(cam->v), viewport_height);
	cam->pixel_delta_u = vec3divscalar(cam->viewport_u, cam->img_width);
	cam->pixel_delta_v = vec3divscalar(cam->viewport_v, cam->img_height);
	part = vec3substr((vec3substr(cam->orig, vec3multscalar(cam->w,
						focal_length))), vec3divscalar(cam->viewport_u, 2));
	viewport_upper_left = vec3substr(part, vec3divscalar(cam->viewport_v, 2));
	cam->pixel00_loc = vec3add(viewport_upper_left,
			vec3divscalar(vec3add(cam->pixel_delta_u, cam->pixel_delta_v), 2));
}

void	update_cam_resize(t_camera *cam, int new_width, int new_height)
{
	t_point3	viewport_upper_left;
	double		foc_len;
	double		viewport_w;
	double		viewport_h;

	if (cam->dir.x == 0 && cam->dir.z == 0)
		cam->dir.x -= 0.1;
	cam->img_width = new_width;
	cam->img_height = new_height;
	cam->aspect_ratio = (double)new_width / new_height;
	foc_len = length(vec3substr(cam->orig, vec3add(cam->orig, cam->dir)));
	viewport_w = 2 * (tan((degrees_to_radians(cam->hfov)) / 2)) * foc_len;
	viewport_h = viewport_w * ((double)cam->img_height / cam->img_width);
	cam->w = unit_vector(vec3substr(cam->orig, vec3add(cam->orig, cam->dir)));
	cam->u = unit_vector(cross(cam->vup, cam->w));
	cam->v = cross(cam->w, cam->u);
	cam->viewport_u = vec3multscalar(cam->u, viewport_w);
	cam->viewport_v = vec3multscalar(vec3negate(cam->v), viewport_h);
	cam->pixel_delta_u = vec3divscalar(cam->viewport_u, cam->img_width);
	cam->pixel_delta_v = vec3divscalar(cam->viewport_v, cam->img_height);
	viewport_upper_left = vec3substr(vec3substr((vec3substr(cam->orig, \
	vec3multscalar(cam->w, foc_len))), vec3divscalar(cam->viewport_u, 2)), \
	vec3divscalar(cam->viewport_v, 2));
	cam->pixel00_loc = vec3add(viewport_upper_left,
			vec3divscalar(vec3add(cam->pixel_delta_u, cam->pixel_delta_v), 2));
}
