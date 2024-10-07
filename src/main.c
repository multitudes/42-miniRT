/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 17:31:01 by lbrusa            #+#    #+#             */
/*   Updated: 2024/10/07 16:52:16 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "debug.h"
#include "libft.h"
#include "texture.h"
#include "minirt.h"
#include "camera.h"
#include "vec3.h"
#include <MLX42/MLX42.h>
#include "utils.h"
#include "color.h"
#include "ambient.h"
#include "quad.h"
#include "plane.h"
#include "disk.h"
#include "box.h"
#include "triangle.h"
#include <time.h>
#include "mersenne_twister.h"
#include "rtw_stb_image.h"
#include "hook_utils.h"


#define WINDOW_TITLE "miniRT"
#define BPP sizeof(int32_t)
#define TRUE 1
#define FALSE 0

int render_from_file(char *filename);
int init_window(t_mrt *data);
bool init_data(t_mrt *data);

void	parse_input(char *filename, t_mrt *data);

int main(int argc, char **argv)
{
	if (argc > 1 && argc < 3)
		render_from_file(argv[1]);
	else
		ft_printf("Usage: %s <filename>\n", argv[0]);
}



int init_window(t_mrt *data)
{

    data->mlx = mlx_init(data->cam.img_width, data->cam.img_height, WINDOW_TITLE, true);
	if (data->mlx == NULL)
		return (FALSE);
	data->image = mlx_new_image(data->mlx, (uint32_t)data->cam.img_width, (uint32_t)data->cam.img_height);
	if (!(data->image))
	{
		mlx_close_window(data->mlx);
		ft_printf("%s\n", mlx_strerror(mlx_errno));
	}
	ft_memset(data->image->pixels, (int)0, (size_t)data->cam.img_width * (size_t)data->cam.img_height * (size_t)BPP);
	if (mlx_image_to_window(data->mlx, data->image, 0, 0) == -1)
	{
		mlx_close_window(data->mlx);
		mlx_terminate(data->mlx);
		ft_printf("%s\n", mlx_strerror(mlx_errno));
    }
	// cursor, why not?
	void *cursor = mlx_create_std_cursor(MLX_CURSOR_CROSSHAIR);
	mlx_set_cursor(data->mlx, cursor);
	// mlx_texture_t* icon_image = mlx_xpm_file_to_image(data->mlx, "assets/42_icon.xpm", 0, 0);
	// mlx_set_icon(data->mlx, icon_image);
	debug("Window initialized");
    return (TRUE);
}

bool init_data(t_mrt *data)
{
	/***************************** */
	/* 			MLX42 			   */
	/***************************** */
    data->mlx = NULL;
    data->win_ptr = NULL;
	data->cores_str = NULL;
	data->seconds_str = NULL;

	data->mlx_time = 0;
    data->image = NULL;
	data->renderscene = render;
	data->needs_render = true;
	// this is for the alternative random algorithm
	// but unsure if it makes sense to use it
	if (BONUS)
		mt_init_genrand(time(NULL));
    return (true);
}

/*
This is the callback of
mlx_resize_hook(params.mlx, &_resize_hook, (void*)&params);
The prototype of the function is given already.
I receive the new height and width from the system.
This works when resizing the window with the handles and also when going in
fullscreen mode for some reason, even if the full screen mode
is controlled differently in the background by the system.
*/
void	_resize_hook(int new_width, int new_height, void *params)
{
	t_mrt *data = ((t_mrt *)params);
	data->cam.img_width = new_width;
	data->cam.img_height = new_height;
	update_cam_resize(&data->cam, new_width, new_height);
	mlx_resize_image(data->image, (uint32_t)new_width, (uint32_t)new_height);
	data->needs_render = true;
}

int render_from_file(char *filename)
{
    t_mrt data;

	if (BONUS)
	{
		debug("Bonus is enabled\n");
		long num_cores = sysconf(_SC_NPROCESSORS_ONLN);
		debug("Number of cores: %ld\n", num_cores);
		data.cam.cores = (uint8_t)num_cores;
	}
	ft_memset(&data, 0, sizeof(t_mrt));
	parse_input(filename, &data);


    debug("Start of minirt %s", "helllo !! ");

	if (!init_window(&data))
		return (EXIT_FAILURE);

	render(&data, &data.world, &data.lights);
	mlx_resize_hook(data.mlx, &_resize_hook, (void *)&data);
    mlx_loop_hook(data.mlx, &hook, (void *)&data);

    mlx_loop(data.mlx);

    ft_printf("\nbyebye!\n");
    mlx_terminate(data.mlx);

    return (EXIT_SUCCESS);


}
