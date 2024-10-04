/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 15:12:47 by lbrusa            #+#    #+#             */
/*   Updated: 2024/10/04 15:16:32 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "camera.h"
#include "bilateral_filter.h"
#include "debug.h"
#include "minirt.h"
#include "ray.h"
#include "utils.h"
#include "vec3.h"

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
