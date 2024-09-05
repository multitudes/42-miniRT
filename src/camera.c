/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 10:28:07 by lbrusa            #+#    #+#             */
/*   Updated: 2024/09/05 19:43:21 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include "minirt.h"
#include "camera.h"
#include "vec3.h"
#include "color.h"
#include "ray.h"
#include "sphere.h"
#include "hittable_list.h"
#include "interval.h"
#include "hittable.h"
#include <MLX42/MLX42.h>
#include "debug.h"



t_color	ray_color(t_ray *r, int depth, const t_hittablelist *world)
{
	t_hit_record rec;

	if (depth <= 0)
		return color(0, 0, 0);
	if (hit_world(world, r, interval(0.001, INFINITY), &rec))
	{
		t_vec3 direction = vec3add(rec.normal, random_unit_vector());
		t_ray new_ray = ray(rec.p, direction);
		return vec3multscalar(ray_color(&new_ray, depth - 1, world), 0.3);

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
    offset = y * IMAGE_WIDTH + x;
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
    while (y < IMAGE_HEIGHT)
    {	
		x = 0;
        while (x < IMAGE_WIDTH)
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
		debug("%.3d of %.3d\r", y, IMAGE_HEIGHT);
		y++;
    }
	debug("\nDONE!\n");
}


// /*
//  * camera
//  * 
//  * aspect_ratio: width / height
//  * image_width: width of the image in pixels
//  * image_height: height of the image in pixels
//  * center: the point in 3D space from which all scene rays will originate
//  * pixel00_loc: the location of the upper left pixel
//  * pixel_delta_u: the offset to the first pixel center
//  * pixel_delta_v: the offset to the pixel center
//  */
// t_camera camera()
// {
// 	t_camera c;
	
// 	// public members
// 	c.aspect_ratio = (double)16.0 / 9.0; 	// Ratio of image width over height
// 	c.image_width = 1200; 					// Rendered image width in pixel count
//     c.samples_per_pixel = 500;				// Count of random samples for each pixel
// 	c.max_depth = 50;						// Maximum number of ray bounces into scene
	
// 	c.vfov = 20; 							// Vertical view angle (field of view)
//     c.lookfrom = point3(13,2,3);			// Point camera is looking from
//     c.lookat = point3(0,0,0);				// Point camera is looking at
//     c.vup = vec3(0,1,0);					// Camera-relative "up" direction
	
//     c.defocus_angle = 0.6; 					// Variation angle of rays through each pixel
//     c.focus_dist = 10;						// Distance from camera lookfrom point to plane of perfect focus

// 	// initialize the camera from public members
// 	c.image_height = (double)c.image_width / c.aspect_ratio;
// 	c.image_height = (c.image_height < 1) ? 1 : c.image_height;
  	
// 	c.pixel_samples_scale = 1.0 / c.samples_per_pixel;

// 	c.center = c.lookfrom;
	
// 	double theta = degrees_to_radians(c.vfov);
//     double h = tan(theta/2);
//     double viewport_height = 2 * h * c.focus_dist;
// 	double viewport_width = viewport_height * ((double)c.image_width/c.image_height);
	
// 	// Calculate the u,v,w unit basis vectors for the camera coordinate frame.
//     c.w = unit_vector(vec3substr(c.lookfrom, c.lookat));
//     c.u = unit_vector(vec3cross(c.vup, c.w));
//     c.v = vec3cross(c.w, c.u);

// 	// Calculate the vectors across the horizontal and down the vertical viewport edges.
//     t_vec3 viewport_u = vec3multscalar(c.u, viewport_width);    // Vector across viewport horizontal edge
// 	t_vec3 viewport_v = vec3multscalar(vec3negate(c.v), viewport_height);  // Vector down viewport vertical edge

// 	 // Calculate the horizontal and vertical delta vectors from pixel to pixel.
//     c.pixel_delta_u  = vec3divscalar(viewport_u, c.image_width);
//     c.pixel_delta_v = vec3divscalar(viewport_v, c.image_height);

//     t_point3 viewport_upper_left = vec3substr(c.center, vec3multscalar(c.w, c.focus_dist));
// 	viewport_upper_left = vec3substr(viewport_upper_left, vec3divscalar(viewport_u, 2));
// 	viewport_upper_left = vec3substr(viewport_upper_left, vec3divscalar(viewport_v, 2));
	
// 	t_vec3 small_translation = vec3add(vec3multscalar(c.pixel_delta_u, 0.5), vec3multscalar(c.pixel_delta_v, 0.5));

//     c.pixel00_loc = vec3add(viewport_upper_left, small_translation);

// 	// Calculate the camera defocus disk basis vectors.
// 	double defocus_radius = c.focus_dist * tan(degrees_to_radians(c.defocus_angle / 2));
// 	c.defocus_disk_u = vec3multscalar(c.u, defocus_radius);
// 	c.defocus_disk_v = vec3multscalar(c.v, defocus_radius);

// 	printf("pixel00_loc: ");
// 	print_vec3(&c.pixel00_loc);

// 	return c;
// }


// void	render(t_camera c, const t_hittablelist world)
// {
// 	// render
// 	// for the book course we create a ppm image
// 	// create_ppm_image("test.ppm", WIDTH, HEIGHT);

// 	FILE *file;
// 	char filepath[PATH_MAX];

// 	// render
// 	sprintf(filepath, "assets/%s", "test2.ppm");
// 	file = fopen(filepath, "w");
// 	fprintf(file, "P3\n%d %d\n255\n", c.image_width, c.image_height);
// 	for (int j = 0; j < c.image_height; j++) 
// 	{
// 		// write to std err
// 		// fprintf(stderr, "\rScanlines remaining: %d\n", image_height - j);
// 		for (int i = 0; i < c.image_width; i++)
// 		{	
// 			t_color pixel_color = color(0,0,0);
//             for (int sample = 0; sample < c.samples_per_pixel; sample++) {
//                     t_ray r = get_ray(&c, i, j);
// 					t_color partial = ray_color(&r, c.max_depth, &world);
//                     pixel_color = vec3add(pixel_color, partial);
//                 }
// 			write_color(file, vec3multscalar(pixel_color, c.pixel_samples_scale));
// 		}
// 	}
// 	fclose(file);
// }


// t_color	ray_color(t_ray *r, const int depth, const t_hittablelist *world)
// {
// 	t_hit_record rec;
// 	if (depth <= 0)
//         return color(0,0,0);
// 	if ((world)->hit(world, r, interval(0.001, INFINITY), &rec))
// 	{
// 		t_ray scattered;
// 		t_color attenuation;
// 		if (rec.mat->scatter(rec.mat, r, &rec, &attenuation, &scattered))
// 			return vec3mult(attenuation, ray_color(&scattered, depth - 1, world));
// 		return color(0,0,0);
// 	}
// 	t_vec3 unit_direction = unit_vector(r->dir);
// 	double a = 0.5 * (unit_direction.y + 1.0);
// 	t_color start = vec3multscalar(color(1.0, 1.0, 1.0), 1.0 - a);
// 	t_color end = vec3multscalar(color(0.5, 0.7, 1.0), a);
// 	return vec3add(start, end);
// }

// /*
//  * get_ray
//  * 
//  * c: camera
//  * i: x coordinate
//  * j: y coordinate
//  * 
//  * returns: a camera ray originating from the defocus disk and directed at a randomly 
//  * sampled point around the pixel location i, j.
//  */
// t_ray get_ray(t_camera *c, int i, int j)  
// {
// 	t_vec3	offset = sample_square();
// 	t_vec3	pixel_sample = vec3add(c->pixel00_loc, 
// 						vec3add(vec3multscalar(c->pixel_delta_u, (i + offset.x)),
// 						vec3multscalar(c->pixel_delta_v, (j + offset.y))));

// 	t_vec3 ray_origin = (c->defocus_angle <= 0) ? c->center : defocus_disk_sample(c);
// 	t_vec3 ray_direction = vec3substr(pixel_sample, ray_origin);

// 	return ray(ray_origin, ray_direction);
// }

// /*
//  * sample_square
//  * 
//  * Returns a random point in the unit square.
//  */
// t_vec3	sample_square()
// {
// 	return vec3(random_d() - 0.5, random_d() - 0.5, 0);
// }

// /*
//  * defocus_disk_sample
//  * 
//  * Returns a random point in the camera defocus disk.
//  */
// t_point3 defocus_disk_sample(t_camera *c)
// {
// 	t_point3 p = random_in_unit_disk();
// 	return vec3add(vec3add(c->center, vec3multscalar(c->defocus_disk_u, p.x)), vec3multscalar(c->defocus_disk_v, p.y));
// }


