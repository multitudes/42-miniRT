/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera_ray_color.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 17:07:38 by lbrusa            #+#    #+#             */
/*   Updated: 2024/10/03 18:12:17 by lbrusa           ###   ########.fr       */
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
#include "bilateral_filter.h"

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
t_color	ray_color(t_scene scene, t_ray *r, int depth)
{
	t_hit_record rec;
	t_scatter_record srec;
	t_hittable_pdf light_pdf;
	t_pdf *recorded_pdf;
	
	if (depth <= 0)
        return color(0,0,0);
	if (!scene.world->hit_objects(scene.world, r, interval(0.001, 100000), &rec))
		return color(0.0,0.0,0.0);
	t_color color_from_emission = rec.mat->emit(rec.mat, rec, rec.uv, rec.p);
	init_scatter_record(&srec);
	if (!rec.mat->scatter(rec.mat, r, &rec, &srec))
		return color_from_emission;
		
	t_ray scattered = srec.skip_pdf_ray;

	if (srec.skip_pdf) {
		t_color ambient_light = scene.cam->ambient.color;
		t_metal *metal = (t_metal *)rec.mat;
		t_color ambient_material = vec3multscalar(vec3add(metal->albedo, vec3multscalar(ambient_light, 0.1)), 0.01);
		t_color reflected_color = vec3mult(srec.attenuation, ray_color(scene, &scattered, depth - 1));
		return vec3add(ambient_material, reflected_color);
    }

	recorded_pdf = srec.pdf_ptr;
	hittable_pdf_init(&light_pdf, scene.lights, &rec.p);
	t_mixture_pdf mix_pdf;
	mixture_pdf_init(&mix_pdf, recorded_pdf, (t_pdf *)&light_pdf);
	scattered = ray(rec.p, mixture_pdf_generate(&mix_pdf));
	double pdf_value = mixture_pdf_value(&mix_pdf, &scattered.dir);
	double scattering_pdf = rec.mat->scattering_pdf(rec.mat, r, &rec, &scattered);
	t_color sample_color = ray_color(scene, &scattered, depth-1);
	t_color ambient = scene.cam->ambient.color;
	t_color ambient_samplecolor = vec3add(ambient, sample_color);
	t_color attenuationxscattering_pdf = vec3multscalar(srec.attenuation, scattering_pdf);
	t_color color_from_scatter_partial = vec3mult(attenuationxscattering_pdf, ambient_samplecolor);
	t_color color_from_scatter = vec3divscalar(color_from_scatter_partial, pdf_value);
	return color_from_scatter;
}

/**
 * @brief gamma correction
 * @param c the color 
 * @return t_color the gamma corrected color
 *
 * 
 * gamma corrects the colorvector.  
 * squareroots the color values. 
 */
uint32_t    color_gamma_corrected(t_color color)
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
    offset = y * data->cam.img_width + x;
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
	t_point3 ray_origin = cam.orig;
	t_vec3 ray_direction = vec3substr(pixel_sample, ray_origin);
	return ray(ray_origin, ray_direction);
}



void render_thread(void *args)
{
	t_thread_data *thread_data;
	thread_data = (t_thread_data *)args;
	int y = thread_data->starty;
	int x = 0;
	int i = 0;
    while (y < thread_data->endy)
    {
		x = 0;
        while (x < thread_data->data->cam.img_width)
        {
			t_color pixel_color = color(0,0,0);
			i = 0;
			while (i < thread_data->data->cam.samples_per_pixel)
			{
				t_ray r = get_ray(thread_data->data->cam, x, y);

				pixel_color = vec3add(pixel_color, ray_color(thread_data->scene, &r, thread_data->data->cam.max_depth));
				i++;
			}

            write_color(thread_data->data, x, y, vec3divscalar(pixel_color, thread_data->data->cam.samples_per_pixel));
			x++;
        }
		y++;
    }
}

/**
 * @brief render the scene
 */
void    render(t_mrt *data, const t_hittablelist* world, const t_hittablelist* lights)
{
    double start_time = mlx_get_time();
    mlx_delete_image(data->mlx, data->seconds_str);
    mlx_delete_image(data->mlx, data->cores_str);
	    // Initialize the scene
    
    int sliceheight = data->cam.img_height / data->cam.cores;
	int thread_idx = 0;
	while (thread_idx < data->cam.cores)
	{
		data->cam.thread_data[thread_idx].data = data;
		data->cam.thread_data[thread_idx].thread_id = thread_idx;
		data->cam.thread_data[thread_idx].scene.world = world;
		data->cam.thread_data[thread_idx].scene.lights = lights;
		data->cam.thread_data[thread_idx].scene.cam = &data->cam;
		data->cam.thread_data[thread_idx].starty = thread_idx * sliceheight;
		if (thread_idx == data->cam.cores - 1)
			data->cam.thread_data[thread_idx].endy = data->cam.img_height;
		else
			data->cam.thread_data[thread_idx].endy = (thread_idx + 1) * sliceheight;
		pthread_create(&data->cam.threads[thread_idx], NULL, (void *)render_thread, &data->cam.thread_data[thread_idx]);
		thread_idx++;
	}
	thread_idx = 0;
	while (thread_idx < data->cam.cores)
	{
		pthread_join(data->cam.threads[thread_idx], NULL);
		thread_idx++;
	}
	if (data->needs_render)
		data->needs_render = false;

	debug("DONE\n");
    
    // Calculate time taken and FPS
    double time_taken = ((double)(mlx_get_time() - start_time));
    char time_str[25];
    char cores_str[25]; 
    sprintf(cores_str, "cores: %d", data->cam.cores);
    sprintf(time_str, "time: %.1f sec", time_taken);

    apply_bilateral_filter(data);

	data->seconds_str =  mlx_put_string(data->mlx, time_str, 10, 10);
    data->cores_str = mlx_put_string(data->mlx, cores_str, 10, 30);
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
	c->orig.x, c->orig.y, c->orig.z,
	c->dir.x, c->dir.y, c->dir.z,
	c->hfov);
}

// Check if two floating-point numbers are approximately equal
bool is_near_zero(double value) {
    return fabs(value) < EPSILON;
}
