/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 17:31:01 by lbrusa            #+#    #+#             */
/*   Updated: 2024/10/14 09:35:32 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ambient.h"
#include "box.h"
#include "camera.h"
#include "color.h"
#include "debug.h"
#include "disk.h"
#include "hook_utils.h"
#include "libft.h"
#include "mersenne_twister.h"
#include "minirt.h"
#include "plane.h"
#include "quad.h"
#include "rtw_stb_image.h"
#include "texture.h"
#include "triangle.h"
#include "utils.h"
#include "vec3.h"
#include <MLX42/MLX42.h>
#include <time.h>
#include "hook_utils.h"

#define BPP 4
#define TRUE 1
#define FALSE 0

static int		render_from_file(char *filename);
static int		init_window(t_mrt *data);

int	main(int argc, char **argv)
{
	if (argc > 1 && argc < 3)
		render_from_file(argv[1]);
	else
		ft_printf("Usage: %s <filename>\n", argv[0]);
}

static int	init_window(t_mrt *data)
{
	void	*cursor;

	data->mlx = mlx_init(data->cam.img_width, data->cam.img_height,
			data->win_title, true);
	if (data->mlx == NULL)
		return (FALSE);
	data->image = mlx_new_image(data->mlx, (uint32_t)data->cam.img_width,
			(uint32_t)data->cam.img_height);
	if (!(data->image))
	{
		mlx_close_window(data->mlx);
		ft_printf("%s\n", mlx_strerror(mlx_errno));
	}
	ft_memset(data->image->pixels, (int)0, (size_t)data->cam.img_width
		* (size_t)data->cam.img_height * (size_t)BPP);
	if (mlx_image_to_window(data->mlx, data->image, 0, 0) == -1)
	{
		mlx_close_window(data->mlx);
		mlx_terminate(data->mlx);
		ft_printf("%s\n", mlx_strerror(mlx_errno));
	}
	cursor = mlx_create_std_cursor(MLX_CURSOR_CROSSHAIR);
	mlx_set_cursor(data->mlx, cursor);
	debug("Window initialized");
	return (TRUE);
}

static int	render_from_file(char *filename)
{
	t_mrt	data;
	long	num_cores;

	if (BONUS)
	{
		ft_printf("Bonus is enabled\n");
		num_cores = sysconf(_SC_NPROCESSORS_ONLN);
		debug("Number of cores: %ld\n", num_cores);
	}
	ft_memset(&data, 0, sizeof(t_mrt));
	parse_input(filename, &data);
	debug("input file parsed: %s", data.win_title);
	if (!init_window(&data))
		return (EXIT_FAILURE);
	render(&data, &data.world, &data.lights);
	mlx_resize_hook(data.mlx, &resize_hook, (void *)&data);
	mlx_loop_hook(data.mlx, &hook, (void *)&data);
	mlx_loop(data.mlx);
	ft_printf("\nbyebye!\n");
	mlx_terminate(data.mlx);
	return (EXIT_SUCCESS);
}
