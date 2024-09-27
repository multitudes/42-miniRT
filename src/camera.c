/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 10:28:07 by lbrusa            #+#    #+#             */
/*   Updated: 2024/09/27 11:24:57 by lbrusa           ###   ########.fr       */
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

void apply_bilateral_filter_to_image(t_mrt *data);

/**
 * @brief update a camera object when the orientation changes
 */
void update_cam_orientation(t_camera *cam)
{
	if (cam->direction.x == 0 && cam->direction.z == 0)
		cam->direction.x -= 0.1;
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
	if (cam->direction.x == 0 && cam->direction.z == 0)
		cam->direction.x -= 0.1;
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

// TODO: what happens in direction vector is 0,0,0 at the start?
void	init_cam(t_camera *cam, t_point3 center, t_vec3 direction, double hfov)
{
	if (direction.x == 0 && direction.z == 0)
		direction.z -= 0.1;
	cam->direction = unit_vector(direction);
	cam->original_dir = cam->direction;
	cam->original_pos = center;
	cam->samples_per_pixel = 100;
	cam->max_depth = 200;
	cam->aspect_ratio = ASPECT_RATIO;
	cam->image_width = IMAGE_WIDTH;
	cam->image_height = (int)(IMAGE_WIDTH / cam->aspect_ratio);
	if (cam->image_height < 1)
		cam->image_height = 1;
	cam->center = center;
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
	// debug("ambient light = %f, %f, %f\n", cam->ambient.color.r, cam->ambient.color.g, cam->ambient.color.b);
	
	if (depth <= 0)
        return color(0,0,0);
	if (!world->hit_objects(world, r, interval(0.001, 100000), &rec))
		return color(0.0,0.0,0.0);
	t_color color_from_emission = rec.mat->emit(rec.mat, rec, rec.u, rec.v, rec.p);
	init_scatter_record(&srec);
	if (!rec.mat->scatter(rec.mat, r, &rec, &srec))
	{
		// debug("light source\n");
		return color_from_emission;
	}
	t_ray scattered = srec.skip_pdf_ray;

	if (srec.skip_pdf) {
		// debug("metal\n"); 
		t_color ambient_light = cam->ambient.color;
		t_metal *metal = (t_metal *)rec.mat;
		t_color ambient_material = vec3multscalar(vec3add(metal->albedo, vec3multscalar(ambient_light, 0.1)), 0.01);
		t_color reflected_color = vec3mult(srec.attenuation, ray_color(cam, &scattered, depth - 1, world, lights));
		return vec3add(ambient_material, reflected_color);
        // return vec3mult(srec.attenuation, ray_color(cam, &scattered, depth - 1, world, lights));
	}

	// debug("not light source or metal\n");
	recorded_pdf = srec.pdf_ptr;
	// i sample the light sources in the hittable pdf init function
	hittable_pdf_init(&light_pdf, lights, &rec.p);
	t_mixture_pdf mix_pdf;
	mixture_pdf_init(&mix_pdf, recorded_pdf, (t_pdf *)&light_pdf);
	scattered = ray(rec.p, mixture_pdf_generate(&mix_pdf));
	double pdf_value = mixture_pdf_value(&mix_pdf, &scattered.dir);
	// scattered = ray(rec.p, recorded_pdf->generate(recorded_pdf));
	// double pdf_value = recorded_pdf->value(recorded_pdf, &scattered.dir);
	double scattering_pdf = rec.mat->scattering_pdf(rec.mat, r, &rec, &scattered);
	t_color sample_color = ray_color(cam, &scattered, depth-1, world, lights);
	// debug("pdf_value = %f, scattering_pdf = %f\n", pdf_value, scattering_pdf);
	// t_color ambient = vec3divscalar(cam->ambient.color,1);
	t_color ambient = cam->ambient.color;
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
	t_thread_data *thread_data;
	thread_data = (t_thread_data *)args;
	int y = thread_data->starty;
	int x = 0;
	int i = 0;
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
}

/**
 * @brief render the scene
 */
void    render(t_mrt *data, const t_hittablelist* world, const t_hittablelist* lights)
{
    double fps; 	// remove later
    double start_time = mlx_get_time();
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

	debug("DONE\n");
    
    // Calculate time taken and FPS
    double time_taken = ((double)(mlx_get_time() - start_time));
    fps = 1.0 / time_taken;
    // char *fps_str = {"Hello"};
    apply_bilateral_filter_to_image(data);
	// mlx_put_string(data->mlx, fps_str, 10, 10);
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




double gaussian(double x, double sigma) {
    return exp(-(x * x) / (2 * sigma * sigma)) / (2 * PI * sigma * sigma);
}

t_rgb bilateral_filter_pixel(mlx_image_t *image, int x, int y, double sigma_s, double sigma_r) {
    double rs = 0, gs = 0, bs = 0;
    double w_sum = 0;

    int radius = (int)(2 * sigma_s);
    for (int i = -radius; i <= radius; ++i) {
        for (int j = -radius; j <= radius; ++j) {
            int nx = x + i;
            int ny = y + j;

            if (nx >= 0 && nx < image->width && ny >= 0 && ny < image->height) {
                uint8_t *neighbor_pixel = &image->pixels[(ny * image->width + nx) * 4];
                uint8_t *center_pixel = &image->pixels[(y * image->width + x) * 4];

                t_rgb neighbor = {neighbor_pixel[0], neighbor_pixel[1], neighbor_pixel[2]};
                t_rgb center = {center_pixel[0], center_pixel[1], center_pixel[2]};

                double spatial_weight = gaussian(sqrt(i * i + j * j), sigma_s);
                double range_weight = gaussian(sqrt(
                    (neighbor.r - center.r) * (neighbor.r - center.r) +
                    (neighbor.g - center.g) * (neighbor.g - center.g) +
                    (neighbor.b - center.b) * (neighbor.b - center.b)
                ), sigma_r);

                double weight = spatial_weight * range_weight;

                rs += neighbor.r * weight;
                gs += neighbor.g * weight;
                bs += neighbor.b * weight;
                w_sum += weight;
            }
        }
    }

    t_rgb result;
    result.r = (uint8_t)(rs / w_sum);
    result.g = (uint8_t)(gs / w_sum);
    result.b = (uint8_t)(bs / w_sum);
    return result;
}

void apply_bilateral_filter(mlx_image_t *image, double sigma_s, double sigma_r) {
    t_rgb *filtered_pixels = (t_rgb *)malloc(image->width * image->height * sizeof(t_rgb));

    for (int y = 0; y < image->height; ++y) {
        for (int x = 0; x < image->width; ++x) {
            filtered_pixels[y * image->width + x] = bilateral_filter_pixel(image, x, y, sigma_s, sigma_r);
        }
    }

    for (int y = 0; y < image->height; ++y) {
        for (int x = 0; x < image->width; ++x) {
            int offset = y * image->width + x;
            uint8_t *pixel = &image->pixels[offset * 4];
            pixel[0] = filtered_pixels[offset].r;
            pixel[1] = filtered_pixels[offset].g;
            pixel[2] = filtered_pixels[offset].b;
        }
    }

    free(filtered_pixels);
}


/**
 * @brief apply the bilateral filter to the image
 * 
 * sigma_s (Spatial Sigma): Controls the spatial extent of the filter. 
 * Increasing sigma_s will make the filter consider a larger neighborhood, 
 * resulting in stronger smoothing.
 * sigma_r (Range Sigma): Controls the intensity difference that the filter considers. 
 * Increasing sigma_r will make the filter less sensitive to intensity differences, 
 * resulting in stronger smoothing.
 */
void apply_bilateral_filter_to_image(t_mrt *data) {
    // Apply the bilateral filter
	// debug("apply_bilateral_filter_to_image\n");
    double sigma_s = 2.0; // Spatial sigma
    double sigma_r = 25.0; // Range sigma
    apply_bilateral_filter(data->image, sigma_s, sigma_r);
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





















// this is something to try if we have time
// https://github.com/RenderKit/oidn
/*

void fill_oidn_buffers(t_mrt *data, float *colorPtr)
{
    int width = data->cam.image_width;
    int height = data->cam.image_height;

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            int offset = y * width + x;
            uint8_t *pixel = &data->image->pixels[offset * 4];
            colorPtr[offset * 3 + 0] = pixel[0] / 255.0f;
            colorPtr[offset * 3 + 1] = pixel[1] / 255.0f;
            colorPtr[offset * 3 + 2] = pixel[2] / 255.0f;
        }
    }
}

void apply_oidn_denoising(t_mrt *data)
{
    int width = data->cam.image_width;
    int height = data->cam.image_height;

    // Create an OIDN device
    OIDNDevice device = oidnNewDevice(OIDN_DEVICE_TYPE_DEFAULT);
    oidnCommitDevice(device);

    // Create buffers for input/output images
    OIDNBuffer colorBuf = oidnNewBuffer(device, width * height * 3 * sizeof(float));
    float *colorPtr = (float *)oidnGetBufferData(colorBuf);

    // Fill the input buffers with your rendered image data
    fill_oidn_buffers(data, colorPtr);

    // Create and configure the filter
    OIDNFilter filter = oidnNewFilter(device, "RT");
    oidnSetFilterImage(filter, "color", colorBuf, OIDN_FORMAT_FLOAT3, width, height, 0, 0, 0);
    oidnSetFilterImage(filter, "output", colorBuf, OIDN_FORMAT_FLOAT3, width, height, 0, 0, 0);
    oidnSetFilterBool(filter, "hdr", true);
    oidnCommitFilter(filter);

    // Execute the filter
    oidnExecuteFilter(filter);

    // Check for errors
    const char *errorMessage;
    if (oidnGetDeviceError(device, &errorMessage) != OIDN_ERROR_NONE)
    {
        printf("Error: %s\n", errorMessage);
    }

    // Write the denoised image back to the image buffer
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            int offset = y * width + x;
            uint8_t *pixel = &data->image->pixels[offset * 4];
            pixel[0] = (uint8_t)(colorPtr[offset * 3 + 0] * 255.0f);
            pixel[1] = (uint8_t)(colorPtr[offset * 3 + 1] * 255.0f);
            pixel[2] = (uint8_t)(colorPtr[offset * 3 + 2] * 255.0f);
        }
    }

    // Cleanup
    oidnReleaseBuffer(colorBuf);
    oidnReleaseFilter(filter);
    oidnReleaseDevice(device);
}

then 
	apply_oidn_denoising(&data);
*/
