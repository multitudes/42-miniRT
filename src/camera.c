/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 10:28:07 by lbrusa            #+#    #+#             */
/*   Updated: 2024/09/12 16:55:29 by lbrusa           ###   ########.fr       */
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
#include "utils.h"
#include "camera.h"
#include "pdf.h"
#include <stdbool.h>
#include <math.h>

#define ASPECT_RATIO (double)16.0/16.0
#define IMAGE_WIDTH 400

// Epsilon value for floating-point comparison
#define EPSILON 1e-1

t_camera init_cam(t_point3 center, t_vec3 direction, double hfov) 
{
	t_camera cam;
	cam.background = color(0.6,0.6,0.6); // grey
	cam.samples_per_pixel = 200;
	cam.max_depth = 100; // bouncing ray
	// ratio is not a given from the subject. we can try different values
	// cam.aspect_ratio = (double)16.0/9.0;
	cam.aspect_ratio = ASPECT_RATIO;
	
	// this is for the antialiasing
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


t_color	ray_color(t_camera *cam, t_ray *r, int depth, const t_hittablelist *world, const t_hittablelist *lights)
{
	(void)cam;

	if (depth <= 0)
        return color(0,0,0);

	t_hit_record rec;

	double pdf_value;

	if (!world->hit_objects(world, r, interval(0.001, INFINITY), &rec))
		return color(0.005,0.005,0.005); 

	t_color color_from_emission = rec.mat->emit(rec.mat, rec, rec.u, rec.v, rec.p);

	t_scatter_record srec;
	init_scatter_record(&srec);
	if (!rec.mat->scatter(rec.mat, r, &rec, &srec))
		return color_from_emission;

	t_ray scattered = srec.skip_pdf_ray;
	if (srec.skip_pdf)
	{
		return vec3mult(srec.attenuation, ray_color(cam, &scattered, depth - 1, world, lights));
	}
	// debug("rec normal: %f %f %f\n", rec.normal.x, rec.normal.y, rec.normal.z);

	t_hittable_pdf light_pdf;
	hittable_pdf_init(&light_pdf, lights, &rec.p);

	t_pdf *recorded_pdf = srec.pdf_ptr;

//   mixture_pdf p(light_ptr, srec.pdf_ptr);

//         ray scattered = ray(rec.p, p.generate(), r.time());
//         auto pdf_value = p.value(scattered.direction());

//         double scattering_pdf = rec.mat->scattering_pdf(r, rec, scattered);

//         color sample_color = ray_color(scattered, depth-1, world, lights);
//         color color_from_scatter =
//             (srec.attenuation * scattering_pdf * sample_color) / pdf_value;

//         return color_from_emission + color_from_scatter;



	if (random_d() < 0.5)
	{
		scattered = ray(rec.p, recorded_pdf->generate(recorded_pdf));
	}
	else
	{
		scattered = ray(rec.p, lights->obj_random(lights, &rec.p));
	}
	
	double pdf_value1 = recorded_pdf->value(recorded_pdf, &scattered.dir);
	double pdf_value2 = lights->obj_pdf_value(lights, &rec.p, &scattered.dir);

    pdf_value = 0.5 * pdf_value1 + 0.5 * pdf_value2;

	double scattering_pdf = rec.mat->scattering_pdf(rec.mat, r, &rec, &scattered);

	t_color sample_color = ray_color(cam, &scattered, depth-1, world, lights);

	t_color attenuationxscattering_pdf = vec3multscalar(srec.attenuation, scattering_pdf);
	t_color color_from_scatter_partial = vec3mult(attenuationxscattering_pdf, sample_color);
	t_color color_from_scatter = vec3divscalar(color_from_scatter_partial, pdf_value);

	return vec3add(color_from_emission, color_from_scatter);
	
	//   mixture_pdf p(light_ptr, srec.pdf_ptr);

    //     ray scattered = ray(rec.p, p.generate(), r.time());
    //     auto pdf_value = p.value(scattered.direction());

    //     double scattering_pdf = rec.mat->scattering_pdf(r, rec, scattered);

    //     color sample_color = ray_color(scattered, depth-1, world, lights);
    //     color color_from_scatter =
    //         (srec.attenuation * scattering_pdf * sample_color) / pdf_value;

    //     return color_from_emission + color_from_scatter;


	// t_mixture_pdf mixture_pdf;
	// if (random_d() < 0.5)
	// {
	// 	mixture_pdf = light_pdf.base.generate(&light_pdf);
	// }
	// else
	// {
	// 	mixture_pdf = srec.pdf_ptr->generate(&srec.pdf_ptr);
	// }
	// t_vec3 mixture_pdf;



	// if (random_d() < 0.5)
	// {
	// 	mixture_pdf = light_pdf.base.generate(&light_pdf);
	// }
	// else
	// {
		// mixture_pdf = srec.pdf_ptr->generate(&srec.pdf_ptr);
	// }

	// scattered = ray(rec.p, mixture_pdf);

	// double first_pdf_value = light_pdf.base.value(&light_pdf.base, &scattered.dir);
	// double second_pdf_value = srec.pdf_ptr->value(srec.pdf_ptr, &scattered.dir);
	// double pdf_value =  0.5 * first_pdf_value + 0.5 * second_pdf_value;


	// double scattering_pdf = rec.mat->scattering_pdf(rec.mat, r, &rec, &scattered);

	// t_color sample_color = ray_color(cam, &scattered, depth-1, world, lights);

	// t_color attenuationxscattering_pdf = vec3multscalar(srec.attenuation, scattering_pdf);
	// t_color color_from_scatter_partial = vec3mult(attenuationxscattering_pdf, sample_color);
	// t_color color_from_scatter = vec3divscalar(color_from_scatter_partial, pdf_value);

	// return vec3add(color_from_emission, color_from_scatter);
	
	
	
	
	
	// if (random_d() < 0.5)
	// 	scattered = ray(rec.p, hittable_pdf_generate(&light_pdf));
	// else
	// 	scattered = ray(rec.p, cosine_pdf_generate(&surface_pdf));

    // scattered = ray(rec.p, mixture_pdf_generate(&mix_pdf), r->tm);

    // pdf_val = 0.5 * cosine_pdf_value(&surface_pdf, &scattered.dir) + 0.5 * hittable_pdf_value(&light_pdf, &scattered.dir);

	// double scattering_pdf = rec.mat->scattering_pdf(rec.mat, r, &rec, &scattered);

	// t_color sample_color = ray_color(cam, &scattered, depth-1, world, lights);

	// t_color attenuationxscattering_pdf = vec3multscalar(attenuation, scattering_pdf);
	// t_color color_from_scatter_partial = vec3mult(attenuationxscattering_pdf, sample_color);
	// t_color color_from_scatter = vec3divscalar(color_from_scatter_partial, pdf_val);

	// t_color color_from_scatter = vec3mult(attenuation, ray_color(cam, &scattered, depth - 1, world, lights));
	
	// return vec3add(color_from_emission, color_from_scatter);
	// t_vec3 x_axis = vec3(100,0,0);
	// t_vec3 y_axis = vec3(0,100,0);
	// t_vec3 z_axis = vec3(0,0,100);
	// 	if (ray_intersects_line(r, &x_axis)) {
	// 	return color(1.0, 0.0, 0.0); // Red for x-axis
	// } else if (ray_intersects_line(r, &y_axis)) {
	// 	return color(0.0, 1.0, 0.0); // Green for y-axis
	// } else if (ray_intersects_line(r, &z_axis)) {
	// 	return color(0.0, 0.0, 1.0); // Blue for z-axis
	// }

	// t_vec3 unit_direction = unit_vector(r->dir);
	// double a = 0.5 * (unit_direction.y + 1.0);
	// t_color start = vec3multscalar(color(1.0, 1.0, 1.0), 1.0 - a);
	// t_color end = vec3multscalar(color(0.5, 0.7, 1.0), a);

	// return vec3add(start, end);
	// if (depth <= 0)
	// 	return color(0, 0, 0);

	// t_hit_record rec;
	// if (!hit_world(world, r, interval(0.001, INFINITY), &rec))
	// {
	// 	// t_vec3 unit_direction = unit_vector(r->dir);
	// 	// double a = 0.5 * unit_direction.y + 1.0;
	// 	// t_color white = color(1.0, 1.0, 1.0);
	// 	// t_color blue = color(0.5, 0.7, 1.0);
	// 	// return vec3add(vec3multscalar(white, (1-a)), vec3multscalar(blue, a));
	// 	return cam->background; // it should be the cam beckground but since this is c and raycolor is not a member func i have no access to cam
	// }


	// t_ray scattered;
	// t_color attenuation;
	// t_color color_from_emission = rec.mat->emit(rec.mat, &rec, rec.u, rec.v, rec.p);
	
	// if (!rec.mat->scatter(rec.mat, r, &rec, &attenuation, &scattered, NULL))
	// 	return color_from_emission;
		
	// t_color color_from_scatter = vec3mult(attenuation, ray_color(cam, &scattered, depth-1, world));
	
	// return vec3add(color_from_emission, color_from_scatter);

	// t_ray scattered;
	// t_color attenuation;
	// if (rec.mat->scatter(rec.mat, r, &rec, &attenuation, &scattered, NULL))
	// 	return vec3mult(attenuation, ray_color(cam, &scattered, depth - 1, world));
	// t_vec3 direction = vec3add(rec.normal, random_unit_vector());
	// t_ray scattered = ray(rec.p, direction);
	// return vec3multscalar(ray_color(&scattered, depth - 1, world), 0.5);
	// return color(0,0,0);
	
	// t_vec3 unit_direction = unit_vector(r->dir);
	// double a = 0.5 * unit_direction.y + 1.0;
	// t_color white = color(1.0, 1.0, 1.0);
	// t_color blue = color(0.5, 0.7, 1.0);
	// return vec3add(vec3multscalar(white, (1-a)), vec3multscalar(blue, a));


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

void    render(t_mrt *data, const t_hittablelist* world, const t_hittablelist* lights)
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

				pixel_color = vec3add(pixel_color, ray_color(&(data->cam), &r, data->cam.max_depth ,world, lights));
				
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

// Check if two floating-point numbers are approximately equal
bool is_near_zero(double value) {
    return fabs(value) < EPSILON;
}

// Function to check if a ray intersects an axis-aligned line
bool ray_intersects_line(const t_ray *r, const t_vec3 *axis) {
    // Check for intersection with x-axis (line along the x-axis)
    if (axis->x != 0 && axis->y == 0 && axis->z == 0) {
        // The ray intersects the x-axis when y = 0 and z = 0
        if (!is_near_zero(r->orig.y) || !is_near_zero(r->orig.z)) {
            // Ray origin is not on the yz-plane, so calculate the intersection point
            double t = -r->orig.y / r->dir.y; // Find where y = 0
            double z_at_t = r->orig.z + t * r->dir.z;
            return is_near_zero(z_at_t); // Check if z also equals 0
        }
        return true; // If the ray origin is on the yz-plane
    }

    // Check for intersection with y-axis (line along the y-axis)
    if (axis->x == 0 && axis->y != 0 && axis->z == 0) {
        // The ray intersects the y-axis when x = 0 and z = 0
        if (!is_near_zero(r->orig.x) || !is_near_zero(r->orig.z)) {
            // Ray origin is not on the xz-plane, so calculate the intersection point
            double t = -r->orig.x / r->dir.x; // Find where x = 0
            double z_at_t = r->orig.z + t * r->dir.z;
            return is_near_zero(z_at_t); // Check if z also equals 0
        }
        return true; // If the ray origin is on the xz-plane
    }

    // Check for intersection with z-axis (line along the z-axis)
    if (axis->x == 0 && axis->y == 0 && axis->z != 0) {
        // The ray intersects the z-axis when x = 0 and y = 0
        if (!is_near_zero(r->orig.x) || !is_near_zero(r->orig.y)) {
            // Ray origin is not on the xy-plane, so calculate the intersection point
            double t = -r->orig.x / r->dir.x; // Find where x = 0
            double y_at_t = r->orig.y + t * r->dir.y;
            return is_near_zero(y_at_t); // Check if y also equals 0
        }
        return true; // If the ray origin is on the xy-plane
    }

    // If none of the conditions match, the ray does not intersect the line
    return false;
}