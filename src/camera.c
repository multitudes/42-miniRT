/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 10:28:07 by lbrusa            #+#    #+#             */
/*   Updated: 2024/09/24 12:01:01 by lbrusa           ###   ########.fr       */
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
#include <time.h>
#include <pthread.h>
#include <stdint.h>

/**
 * @brief update a camera object when the orientation changes
 */
void update_cam_orientation(t_camera *cam)
{
	cam->direction = vec3negate(cam->w);
    t_point3 lookat = vec3add(cam->center, cam->direction);
    double focal_length = length(vec3substr(cam->center, lookat));
    double theta = degrees_to_radians(cam->hfov);
    double h = tan(theta / 2);
    double viewport_width = 2 * h * focal_length;
    double viewport_height = viewport_width * ((double)cam->image_height / cam->image_width);

    t_vec3 viewport_u = vec3multscalar(cam->u, viewport_width);
    t_vec3 viewport_v = vec3multscalar(vec3negate(cam->v), viewport_height);
    cam->pixel_delta_u = vec3divscalar(viewport_u, cam->image_width);
    cam->pixel_delta_v = vec3divscalar(viewport_v, cam->image_height);

    t_point3 part1 = vec3substr(cam->center, vec3multscalar(cam->w, focal_length));
    t_point3 part2 = vec3substr(part1, vec3divscalar(viewport_u, 2));
    t_point3 viewport_upper_left = vec3substr(part2, vec3divscalar(viewport_v, 2));
    cam->pixel00_loc = vec3add(viewport_upper_left, vec3divscalar(vec3add(cam->pixel_delta_u, cam->pixel_delta_v), 2));
}

// void update_cam(t_camera *cam, t_point3 center, t_vec3 direction, double hfov)
// {
// 	cam->hfov = hfov;
// 	cam->center = center;
// 	cam->direction = direction;
//     t_point3 lookat = vec3add(cam->center, cam->direction);
// 	double focal_length = length(vec3substr(cam->center, lookat));
// 	double theta = degrees_to_radians(cam->hfov);
//     double h = tan(theta/2);
// 	double viewport_width = 2 * h * focal_length;
//     double viewport_height = viewport_width * ((double)cam->image_height/cam->image_width);
//     cam->w = unit_vector(vec3substr(cam->center, lookat));
//     cam->u = unit_vector(cross(cam->vup, cam->w));
//     cam->v = cross(cam->w, cam->u);
//     t_vec3 viewport_u = vec3multscalar(cam->u, viewport_width);
// 	t_vec3 viewport_v = vec3multscalar(vec3negate(cam->v), viewport_height);
// 	cam->pixel_delta_u = vec3divscalar(viewport_u, cam->image_width);
// 	cam->pixel_delta_v = vec3divscalar(viewport_v, cam->image_height);
// 	t_point3 part1 = vec3substr(cam->center, vec3multscalar(cam->w, focal_length));
// 	t_point3 part2 = vec3substr(part1, vec3divscalar(viewport_u, 2));
// 	t_point3 viewport_upper_left = vec3substr(part2, vec3divscalar(viewport_v, 2));
// 	cam->pixel00_loc = vec3add(viewport_upper_left, vec3divscalar(vec3add(cam->pixel_delta_u, cam->pixel_delta_v), 2));
// }

void update_cam_resize(t_camera *cam, int new_width, int new_height)
{
	cam->image_width = new_width;
	cam->image_height = new_height;
	cam->aspect_ratio = (double)new_width / new_height;
    t_point3 lookat = vec3add(cam->center, cam->direction);
	double focal_length = length(vec3substr(cam->center, lookat));
	double theta = degrees_to_radians(cam->hfov);
    double h = tan(theta/2);
	double viewport_width = 2 * h * focal_length;
    double viewport_height = viewport_width * ((double)cam->image_height/cam->image_width);
    cam->w = unit_vector(vec3substr(cam->center, lookat));
    cam->u = unit_vector(cross(cam->vup, cam->w));
    cam->v = cross(cam->w, cam->u);
    t_vec3 viewport_u = vec3multscalar(cam->u, viewport_width);
	t_vec3 viewport_v = vec3multscalar(vec3negate(cam->v), viewport_height);
	cam->pixel_delta_u = vec3divscalar(viewport_u, cam->image_width);
	cam->pixel_delta_v = vec3divscalar(viewport_v, cam->image_height);
	t_point3 part1 = vec3substr(cam->center, vec3multscalar(cam->w, focal_length));
	t_point3 part2 = vec3substr(part1, vec3divscalar(viewport_u, 2));
	t_point3 viewport_upper_left = vec3substr(part2, vec3divscalar(viewport_v, 2));
	cam->pixel00_loc = vec3add(viewport_upper_left, vec3divscalar(vec3add(cam->pixel_delta_u, cam->pixel_delta_v), 2));
}

void	init_cam(t_camera *cam, t_point3 center, t_vec3 direction, double hfov) 
{
	if (cam->direction.x == 0 && cam->direction.z == 0)
		cam->direction.x += 0.1;
	cam->original_dir = direction;
	cam->original_pos = center;
	cam->samples_per_pixel = 100;
	cam->max_depth = 200;
	cam->aspect_ratio = ASPECT_RATIO;
	cam->image_width = IMAGE_WIDTH; 
	cam->image_height = (int)(IMAGE_WIDTH / cam->aspect_ratio);
	if (cam->image_height < 1)
		cam->image_height = 1;
	cam->center = center;
	cam->direction = unit_vector(cam->direction );
	t_point3 lookat = vec3add(cam->center, cam->direction);
	cam->hfov = clamp(interval(1, 170), hfov);
	ambient(&cam->ambient, 0.2, (t_rgb){.r = 10, .g = 10, .b = 10});
    cam->w = unit_vector(vec3substr(cam->center, lookat));
    cam->u = unit_vector(cross(cam->vup, cam->w));
    cam->v = cross(cam->w, cam->u);
	cam->vup = vec3(0,1,0);
	cam->print = print_camera;
	update_cam_resize(cam, cam->image_width, cam->image_height);
}

/**
 * @brief one of the most important functions in the raytracer
 * this function will return the color of the pixel for a given camera position
 * sending rays into the scene. 
 * @param cam the camera object
 * @param r the ray object
 * @param depth the maximum number of bounces
 * @param world the list of objects in the scene
 * @param lights the list of light sources in the scene
 * @return t_color the color of the pixel
 * 
 * If there are no hits the functions returns a space grey color.
 * Then first I check if the material is a light source and return the color of the light source
 * or what the emit function fort the material returns. Materials like diffuse lambertians and
 * metals will have no emission and the emit function will return a black color.
 * So the next check if the material has a scatter function this will populate the scatter record
 * with the attenuation and the scattered ray. If the material is a light source the scatter function
 * will return false and the function will return the color of the light source.
 * if the material is metal the scatter function will return the scattered ray and the attenuation
 * and the function will call itself recursively with the scattered ray and return the color of the object
 * At this stage i mix the ambient light with the color returned by the recursive call otherwise the metal
 * object will be black with only specular reflections.  
 * If it is not a metal object the function will continue and do a pdf calculation for the light sources.
 * In the hittable_pdf struct I have a pointer to the light source list and the point of intersection.
 * I will do a mix of the pdf values of the light sources and the material and then I will randomly choose
 * a light source or the material to scatter the ray. I will calculate the pdf value for the chosen light source
 * and the material and then I will calculate the scattering pdf for the material. I will call the function
 * recursively with the scattered ray and return the color of the object.
 * 
 */
t_color	ray_color(t_camera *cam, t_ray *r, int depth, const t_hittablelist *world, const t_hittablelist *lights)
{
	t_hit_record rec;
	t_scatter_record srec;
	t_hittable_pdf light_pdf;
	t_pdf *recorded_pdf;

	// base case - stops recursion
	if (depth <= 0)
        return color(0,0,0);
	if (!world->hit_objects(world, r, interval(0.001, 100000), &rec))
		return color(0.01,0.01,0.01);
	t_color color_from_emission = rec.mat->emit(rec.mat, rec, rec.u, rec.v, rec.p);
	init_scatter_record(&srec);
	if (!rec.mat->scatter(rec.mat, r, &rec, &srec))
		return color_from_emission;
	t_ray scattered = srec.skip_pdf_ray;
	// if (srec.skip_pdf)
	// {
	// 	if (srec.skip_pdf) {
	// 		t_color ambient_light = cam->ambient_light.color;
	// 		t_metal *metal = (t_metal *)rec.mat;
	// 		t_color ambient_material = vec3mult(metal->albedo, ambient_light);
    //    		t_color reflected_color = vec3mult(srec.attenuation, ray_color(cam, &scattered, depth - 1, world, lights));
    //     	return vec3add(ambient_material, reflected_color);
    // 	}
	// }
	if (srec.skip_pdf) {
		t_color ambient_light = cam->ambient.color;
		t_metal *metal = (t_metal *)rec.mat;
		t_color ambient_material = vec3mult(metal->albedo, ambient_light);
		t_color reflected_color = vec3mult(srec.attenuation, ray_color(cam, &scattered, depth - 1, world, lights));
		return vec3add(ambient_material, reflected_color);
        // return vec3mult(srec.attenuation, ray_color(cam, &scattered, depth - 1, world, lights));
	}

	recorded_pdf = srec.pdf_ptr;
	hittable_pdf_init(&light_pdf, lights, &rec.p);
	t_mixture_pdf mix_pdf;
	mixture_pdf_init(&mix_pdf, recorded_pdf, (t_pdf *)&light_pdf);
	scattered = ray(rec.p, mixture_pdf_generate(&mix_pdf));
	double pdf_value = mixture_pdf_value(&mix_pdf, &scattered.dir);
	double scattering_pdf = rec.mat->scattering_pdf(rec.mat, r, &rec, &scattered);
	t_color sample_color = ray_color(cam, &scattered, depth-1, world, lights);
	t_color ambient = vec3divscalar(cam->ambient.color,1);
	t_color ambient_samplecolor = vec3add(ambient, sample_color);
	t_color attenuationxscattering_pdf = vec3multscalar(srec.attenuation, scattering_pdf);
	t_color color_from_scatter_partial = vec3mult(attenuationxscattering_pdf, ambient_samplecolor);
	t_color color_from_scatter = vec3divscalar(color_from_scatter_partial, pdf_value);
	return color_from_scatter;
}

/* gamma corrects the colorvector. - squareroots the color values. */
unsigned int    color_gamma_corrected(t_color color)
{
	t_interval intensity;
	
	intensity = interval(0.0,0.999);
	t_rgb a;
	a = color_to_rgb((t_color){
		.r = clamp(intensity, linear_to_gamma(color.r)), 
		.g = clamp(intensity, linear_to_gamma(color.g)), 
		.b = clamp(intensity, linear_to_gamma(color.b))});
    return (rgb_to_uint(a));
}

/*
this is my draw pixel function. I write directly to the buffer
and the color is RGBA or 4 bytes. Code inspired from the MLX42 lib!
*/
void write_color(t_mrt *data, int x, int y, t_color colorvector)
{
    unsigned int color = color_gamma_corrected(colorvector);
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

/* makes a ray, that goes from the cam through (i, j) on the viewport */
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

void render_thread(void *args)
{
	clock_t start_time, end_time; // to remove eventually later
    double time_taken, fps; 	// remove later
	t_thread_data *thread_data;
	thread_data = (t_thread_data *)args;
	int y = thread_data->starty;
	int x = 0;
	int i = 0;
    start_time = clock();
    while (y < thread_data->endy)
    {	
		x = 0;
        while (x < thread_data->data->cam.image_width)
        {
			t_color pixel_color = color(0,0,0);
			i = 0;
			while (i < thread_data->data->cam.samples_per_pixel)
			{
				t_ray r = get_ray(thread_data->data->cam, x, y);

				pixel_color = vec3add(pixel_color, ray_color(&(thread_data->data->cam), &r, thread_data->data->cam.max_depth ,thread_data->world, thread_data->lights));
				
				i++;
			}

            write_color(thread_data->data, x, y, vec3divscalar(pixel_color, thread_data->data->cam.samples_per_pixel));
			x++;
        }
		y++;
    }
	
	end_time = clock();

    // Calculate time taken and FPS
    time_taken = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    fps = 1.0 / time_taken;
	// debug("thread %d  - Frame rendered in %.2f seconds (FPS: %.2f)\n", thread_data->thread_id, time_taken, fps);
	// fflush(stderr);
	// char fps_str[100];
	// snprintf(fps_str, sizeof(fps_str), "Frame rendered in %.2f seconds (FPS: %.2f)", time_taken, fps);
	// mlx_string_put(thread_data->data->mlx, thread_data->data->win_ptr, 10, 10, 0xFFFFFF, fps_str);
	
}


/**
 * @brief render the scene
 */
void    render(t_mrt *data, const t_hittablelist* world, const t_hittablelist* lights)
{
	pthread_t threads[CORES];
	t_thread_data thread_data[CORES];

    int sliceheight = data->cam.image_height / CORES;
	int thread_idx = 0;
	while (thread_idx < CORES)
	{	
		thread_data[thread_idx].data = data;
		thread_data[thread_idx].thread_id = thread_idx;
		thread_data[thread_idx].world = world;
		thread_data[thread_idx].lights = lights;
		thread_data[thread_idx].starty = thread_idx * sliceheight;
		if (thread_idx == CORES - 1)
			thread_data[thread_idx].endy = data->cam.image_height;
		else
			thread_data[thread_idx].endy = (thread_idx + 1) * sliceheight;
		pthread_create(&threads[thread_idx], NULL, (void *)render_thread, &thread_data[thread_idx]);
		thread_idx++;
	}
	thread_idx = 0;
	while (thread_idx < CORES)
	{
		pthread_join(threads[thread_idx], NULL);
		thread_idx++;
	}
	if (data->needs_render)
		data->needs_render = false;
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

// probably i will not need this.. i just wanted to visualize the axes...
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


unsigned int	mlx_get_pixel(mlx_image_t *data, int x, int y)
{
	uint8_t	*dst;

	dst = data->pixels + (y * data->width + x * (sizeof(int32_t) / 8));
	return (*(unsigned int *)dst);
}

/**
 * not working right now but interesting test... just playing around
 * 
 */
void gaussian_blur(t_mrt *data) {
    debug("Applying Gaussian blur to the image...\n");
    int width = data->cam.image_width;
    int height = data->cam.image_height;
    mlx_image_t *image = data->image;
    mlx_image_t *blurred_image = mlx_new_image(data->mlx, width, height);

    // int kernel_size = 5;
    // int kernel_half = kernel_size / 2;
    // double kernel[5][5] = {
    //     {1/273.0, 4/273.0, 7/273.0, 4/273.0, 1/273.0},
    //     {4/273.0, 16/273.0, 26/273.0, 16/273.0, 4/273.0},
    //     {7/273.0, 26/273.0, 41/273.0, 26/273.0, 7/273.0},
    //     {4/273.0, 16/273.0, 26/273.0, 16/273.0, 4/273.0},
    //     {1/273.0, 4/273.0, 7/273.0, 4/273.0, 1/273.0}
    // };

// int kernel_size = 3;
// int kernel_half = kernel_size / 2;
// double kernel[3][3] = {
//     {1.0 / 16.0, 2.0 / 16.0, 1.0 / 16.0},
//     {2.0 / 16.0, 4.0 / 16.0, 2.0 / 16.0},
//     {1.0 / 16.0, 2.0 / 16.0, 1.0 / 16.0}
// };
// int kernel_size = 3;
// int kernel_half = kernel_size / 2;
// double kernel[3][3] = {
//     {-1.0, -2.0, -1.0},
//     {0.0, 0.0, 0.0},
//     {1.0, 2.0, 1.0}
// };
int kernel_size = 3;
int kernel_half = kernel_size / 2;
double kernel[3][3] = {
    {0.0, 0.25, 0.0},
    {0.25, 0.50, 0.25},
    {0.0, 0.25, 0.0}
};
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            t_color accumulated_color = color(0.0, 0.0, 0.0);

            for (int ky = -kernel_half; ky <= kernel_half; ky++) {
                for (int kx = -kernel_half; kx <= kernel_half; kx++) {
                    int px = x + kx;
                    int py = y + ky;

                    if (px >= 0 && px < width && py >= 0 && py < height) {
                        uint8_t *pixel = (uint8_t *)(image->pixels + (py * width + px) * 4);
                        t_color pixel_color = rgb_to_color((t_rgb){pixel[0], pixel[1], pixel[2]});

                        accumulated_color.r += pixel_color.r * kernel[ky + kernel_half][kx + kernel_half];
                        accumulated_color.g += pixel_color.g * kernel[ky + kernel_half][kx + kernel_half];
                        accumulated_color.b += pixel_color.b * kernel[ky + kernel_half][kx + kernel_half];
                    }
                }
            }

            // Clamp accumulated color values to the range 0-1
            accumulated_color.r = fmax(0.0, fmin(accumulated_color.r, 1.0));
            accumulated_color.g = fmax(0.0, fmin(accumulated_color.g, 1.0));
            accumulated_color.b = fmax(0.0, fmin(accumulated_color.b, 1.0));

            t_rgb final_rgb = color_to_rgb(accumulated_color);
            uint32_t blurred_color = (final_rgb.r << 24) | (final_rgb.g << 16) | (final_rgb.b << 8) | 0xFF;
            mlx_put_pixel(blurred_image, x, y, blurred_color);
        }
    }

    // Replace the original image with the blurred image
    mlx_delete_image(data->mlx, data->image);
    data->image = blurred_image;
    mlx_image_to_window(data->mlx, blurred_image, 0, 0);
    debug("Gaussian blur applied to the image!\n");
}