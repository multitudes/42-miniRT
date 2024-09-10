#include "debug.h"
#include "libft.h"
#include "minirt.h"
#include "camera.h"
#include "vec3.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"
#include <MLX42/MLX42.h>
#include "utils.h"
#include "color.h"
#include "ambient.h"

#define WINDOW_TITLE "miniRT"
#define BPP sizeof(int32_t)
#define TRUE 1
#define FALSE 0

void	exit_gracefully(mlx_t *mlx)
{
	ft_printf("byebye!\n");
	mlx_close_window(mlx);
	mlx_terminate(mlx);
	exit(EXIT_SUCCESS);
}

void	hook(void *param)
{
	mlx_t		*mlx;
	t_mrt		*mrt;

	mrt = (t_mrt *)param;
	mlx = mrt->mlx;
	if (mlx_is_key_down(mlx, MLX_KEY_ESCAPE))
		exit_gracefully(mlx);
}

int init_window(t_mrt *data)
{
    data->mlx = mlx_init(data->objects.camera.image_width, data->objects.camera.image_height, WINDOW_TITLE, false);
	if (data->mlx == NULL)
		return (FALSE);
	data->image = mlx_new_image(data->mlx, data->objects.camera.image_width, data->objects.camera.image_height);
	if (!(data->image))
	{
		mlx_close_window(data->mlx);
		ft_printf("%s\n", mlx_strerror(mlx_errno));

	}
	if (mlx_image_to_window(data->mlx, data->image, 0, 0) == -1)
	{
		mlx_close_window(data->mlx);
		ft_printf("%s\n", mlx_strerror(mlx_errno));
    }
	debug("Window initialized");
    return (TRUE);
}

int main(int argc, char **argv)
{
    t_mrt data;
    (void)argv;
	(void)argc;

	ft_memset(&data, 0, sizeof(t_mrt));

	parse_input("example_scene.rt", &data.objects);
	return (0);



	// TODO: needa to create a hittable list
	// list[0] = (t_hittable*)(&s1);
	// list[1] = (t_hittable*)(&s2);
	// list[2] = (t_hittable*)(&s3);
	// list[3] = (t_hittable*)(&s4);

	const t_hittablelist world = hittablelist(list, 4);

    debug("Start of minirt %s", "helllo !! ");
	if (!init_window(&data))
		return (EXIT_FAILURE);

	render(&data, &world);

    mlx_loop_hook(data.mlx, &hook, (void *)&data);

    mlx_loop(data.mlx);
    ft_printf("\nbyebye!\n");
    mlx_terminate(data.mlx);

    return (EXIT_SUCCESS);
}
