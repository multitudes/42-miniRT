/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 10:28:07 by lbrusa            #+#    #+#             */
/*   Updated: 2024/09/07 14:43:43 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <MLX42/MLX42.h>
#include "debug.h"
#include "minirt.h"
#include "vec3.h"
#include "color.h"
#include "ray.h"
#include "hittable_list.h"
#include "interval.h"
#include "hittable.h"
#include "utils.h"
#include "camera.h"

t_camera init_cam(t_point3 center, t_vec3 direction, double hfov) 
{
	t_camera cam;

	// ratio is not a given from the subject. we can try different values
	cam.aspect_ratio = (double)16.0/9.0;
	// cam.aspect_ratio = (double)16.0/16.0;
	
	// this is for the antialiasing
	cam.samples_per_pixel = 10;
	cam.max_depth = 5; // bouncing ray
	cam.image_width = IMAGE_WIDTH; // also not given in the subject or file, smaller is faster - defined in minirt.h
	cam.image_height = IMAGE_WIDTH / cam.aspect_ratio;
	cam.image_height = (cam.image_height < 1) ? 1 : cam.image_height;
	cam.center = center;  // refactor later
	cam.direction = direction;
	cam.lookfrom = center;

	cam.hfov = clamp(interval(1, 170), hfov); // the book has vfow, but we use hfov
	cam.vup = vec3(0,1,0);					// Camera-relative "up" direction
	
	cam.print = print_camera;

    // Calculate lookat from lookdir
    t_point3 lookat = vec3add(cam.lookfrom, direction);

	double focal_length = length(vec3substr(center, lookat));
	
	double theta = degrees_to_radians(hfov);
    double h = tan(theta/2);
	double viewport_width = 2 * h * focal_length;
    double viewport_height = viewport_width * ((double)cam.image_height/cam.image_width);
	
	// Calculate the u,v,w unit basis vectors for the camera coordinate frame.
    cam.w = unit_vector(vec3substr(cam.lookfrom, lookat));
    cam.u = unit_vector(cross(cam.vup, cam.w));
    cam.v = cross(cam.w, cam.u);

	// Calculate the vectors across the horizontal and down the vertical viewport edges.
    t_vec3 viewport_u = vec3multscalar(cam.u, viewport_width);    // Vector across viewport horizontal edge
	t_vec3 viewport_v = vec3multscalar(vec3negate(cam.v), viewport_height);  // Vector down viewport vertical edge

	cam.pixel_delta_u = vec3divscalar(viewport_u, cam.image_width);
	cam.pixel_delta_v = vec3divscalar(viewport_v, cam.image_height);

	t_point3 part1 = vec3substr(cam.center, vec3multscalar(cam.w, focal_length));
	t_point3 part2 = vec3substr(part1, vec3divscalar(viewport_u, 2));
	t_point3 viewport_upper_left = vec3substr(part2, vec3divscalar(viewport_v, 2));

	cam.pixel00_loc = vec3add(viewport_upper_left, vec3divscalar(vec3add(cam.pixel_delta_u, cam.pixel_delta_v), 2));
    return cam;
}

t_color	ray_color(t_ray *r, int depth, const t_hittablelist *world)
{
	if (depth <= 0)
		return color(0, 0, 0);
	t_hit_record rec;
	if (hit_world(world, r, interval(0.001, INFINITY), &rec))
	{
			t_ray scattered;
		t_color attenuation;
		if (rec.mat->scatter(rec.mat, r, &rec, &attenuation, &scattered, NULL))
			return vec3mult(attenuation, ray_color(&scattered, depth - 1, world));
		// t_vec3 direction = vec3add(rec.normal, random_unit_vector());
		// t_ray scattered = ray(rec.p, direction);
		// return vec3multscalar(ray_color(&scattered, depth - 1, world), 0.5);
		return color(0,0,0);
	}
	
	t_vec3 unit_direction = unit_vector(r->dir);
	double a = 0.5 * unit_direction.y + 1.0;
	t_color white = color(1.0, 1.0, 1.0);
	t_color blue = color(0.5, 0.7, 1.0);
	return vec3add(vec3multscalar(white, (1-a)), vec3multscalar(blue, a));


}

unsigned int    color_gamma_corrected(t_color color)
{
	t_interval intensity = interval(0.0,0.999);
	int r = clamp(intensity, linear_to_gamma(color.r)) * 255;
	int g = clamp(intensity, linear_to_gamma(color.g)) * 255;
	int b = clamp(intensity, linear_to_gamma(color.b)) * 255;
    return ((r << 24) | (g << 16) | (b << 8) | 0xFF);
}

/*
this is my draw pixel function. I write directly to the buffer 
and the color is RGBA or 4 bytes. Code inspired from the MLX42 lib!
*/
void write_color(t_mrt *data, int x, int y, t_color colorvector)
{
    int color = color_gamma_corrected(colorvector);
    int offset;
    mlx_image_t *image;
    uint8_t *pixel;

    image = data->image;
    offset = y * data->cam.image_width + x;
    pixel = &image->pixels[offset * 4];
    *(pixel++) = (uint8_t)(color >> 24);
    *(pixel++) = (uint8_t)(color >> 16);
    *(pixel++) = (uint8_t)(color >> 8);
    *(pixel++) = (uint8_t)(color & 0xFF);
}


t_ray get_ray(t_camera cam, int i, int j)
{
	t_vec3 offset = sample_square();
	
	t_vec3 iu = vec3multscalar(cam.pixel_delta_u, i + offset.x);
	t_vec3 ju = vec3multscalar(cam.pixel_delta_v, j + offset.y);
	t_vec3 partial = vec3add(iu, ju); 
	t_point3 pixel_sample = vec3add(cam.pixel00_loc, partial);

	t_point3 ray_origin = cam.center;
	t_vec3 ray_direction = vec3substr(pixel_sample, ray_origin); 
	return ray(ray_origin, ray_direction);

}

void    render(t_mrt *data, const t_hittablelist* world)
{
    int             x;
	int             y;
	int 			i;
    
    y = 0;
    x = 0;
	i = 0;

    while (y < data->cam.image_height)
    {	
		x = 0;
        while (x < data->cam.image_width)
        {
			t_color pixel_color = color(0,0,0);
			i = 0;
			while (i < data->cam.samples_per_pixel)
			{
				t_ray r = get_ray(data->cam, x, y);

				pixel_color = vec3add(pixel_color, ray_color(&r, data->cam.max_depth ,world));
				
				i++;
			}

            write_color(data, x, y, vec3divscalar(pixel_color, data->cam.samples_per_pixel));
			x++;
			// add bar progress
        }
		debug("%.3d of %.3d\r", y, data->cam.image_height);
		y++;
    }
	debug("\nDONE!\n");
}

/** 
 * @brief print the camera information
 * in the rt file format
 * like C -50,0,20 		0,0,1	 70
 */
void			print_camera(const void* self)
{
	const t_camera *c = (const t_camera *)self;
	printf("C\t%.f,%.f,%.f\t\t%.f,%.f,%.f\t\t%.f\n", 
	c->center.x, c->center.y, c->center.z, 
	c->direction.x, c->direction.y, c->direction.z, 
	c->hfov);
}