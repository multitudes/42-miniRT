/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 15:12:47 by lbrusa            #+#    #+#             */
/*   Updated: 2024/10/07 17:29:19 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bilateral_filter.h"
#include "camera.h"
#include "debug.h"
#include "libft.h"
#include "minirt.h"
#include "ray.h"
#include "utils.h"
#include "vec3.h"

t_color	get_pixel_color(t_thread_data *thread_data, int xyi[3])
{
	t_ray	r;
	t_color	pixel_color;

	xyi[2] = 0;
	pixel_color = color(0, 0, 0);
	while (xyi[2] < thread_data->data->cam.samples_per_pixel)
	{
		r = get_ray(thread_data->data->cam, xyi[0], xyi[1]);
		pixel_color = vec3add(pixel_color, ray_color(thread_data->scene, &r,
					thread_data->data->cam.max_depth));
		xyi[2]++;
	}
	return (pixel_color);
}

void	render_thread(void *args)
{
	int				xyi[3];
	t_thread_data	*thread_data;
	t_color			pixel_color;

	thread_data = (t_thread_data *)args;
	xyi[1] = thread_data->starty;
	xyi[0] = 0;
	xyi[2] = 0;
	while (xyi[1] < thread_data->endy)
	{
		xyi[0] = 0;
		while (xyi[0] < thread_data->data->cam.img_width)
		{
			pixel_color = get_pixel_color(thread_data, xyi);
			write_color(thread_data->data, xyi[0], xyi[1],
				vec3divscalar(pixel_color,
					thread_data->data->cam.samples_per_pixel));
			xyi[0]++;
		}
		xyi[1]++;
	}
}

void	update_time(double start_time, t_mrt *data)
{
	int		time_taken;
	char	time_str[25];
	char	cores_str[25];
	char	*temp;

	debug("DONE\n");
	time_taken = ((int)(mlx_get_time() - start_time));
	ft_strlcpy(cores_str, "cores: ", 25);
	temp = ft_itoa(data->cam.cores);
	ft_strlcat(cores_str, temp, 25);
	free(temp);
	ft_strlcpy(time_str, "time: ", 25);
	temp = ft_itoa(time_taken);
	ft_strlcat(time_str, temp, 25);
	ft_strlcat(time_str, " secs", 25);
	free(temp);
	apply_bilateral_filter(data);
	data->seconds_str = mlx_put_string(data->mlx, time_str, 10, 10);
	data->cores_str = mlx_put_string(data->mlx, cores_str, 10, 30);
}

void	innerloop(t_mrt *data, const t_hittablelist *world,
		const t_hittablelist *lights)
{
	int	sliceheight;
	int	thread_idx;

	sliceheight = data->cam.img_height / data->cam.cores;
	thread_idx = 0;
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
			data->cam.thread_data[thread_idx].endy = (thread_idx + 1)
				* sliceheight;
		pthread_create(&data->cam.threads[thread_idx], NULL,
			(void *)render_thread, &data->cam.thread_data[thread_idx]);
		thread_idx++;
	}
}

/**
 * @brief render the scene
 */
void	render(t_mrt *data, const t_hittablelist *world,
		const t_hittablelist *lights)
{
	double	start_time;
	int		thread_idx;

	start_time = mlx_get_time();
	mlx_delete_image(data->mlx, data->seconds_str);
	mlx_delete_image(data->mlx, data->cores_str);
	innerloop(data, world, lights);
	thread_idx = 0;
	while (thread_idx < data->cam.cores)
	{
		pthread_join(data->cam.threads[thread_idx], NULL);
		thread_idx++;
	}
	if (data->needs_render)
		data->needs_render = false;
	update_time(start_time, data);
}
