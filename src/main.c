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
    data->mlx = mlx_init(data->cam.image_width, data->cam.image_height, WINDOW_TITLE, false);
	if (data->mlx == NULL)
		return (FALSE);
	data->image = mlx_new_image(data->mlx, data->cam.image_width, data->cam.image_height);
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

bool init_data(t_mrt *data)
{
	/***************************** */
	/* 			MLX42 			   */	
	/***************************** */
    data->mlx = NULL;
    data->win_ptr = NULL;
    data->image = NULL;

	/***************************** */
	/* 			camera 			   */	
	/***************************** */
	t_point3 center = point3(-2,2,1);
	t_vec3 direction = vec3(2,-2,-2);
	data->cam = init_cam(center, direction, 10);
	data->cam.print((void*)(&(*data).cam));

	/***************************** */
	/* 		ambient light		   */	
	/***************************** */
	t_ambient ambient_light = ambient(0.2, rgb(255,255,255));
	data->ambient_light = ambient_light;
	ambient_light.print((void*)&ambient_light);

    return (true);
}



int main(int argc, char **argv)
{
    t_mrt data;
    (void)argv;
	(void)argc;
	if (!init_data(&data))
        return (1);

	// world
	t_hittable *list[4];

	t_sphere s1 = sphere(vec3(0, 0, -1.2), 1, rgb(128,0,0));
	s1.print((void*)&s1);
	t_sphere s2 = sphere(vec3(0, -100.5, -1), 200, rgb(0,128,0));
	s2.print((void*)&s2);
	t_sphere s3 = sphere(vec3(-1, 0.0, -1.0), 1, rgb(128,128,0));
	s3.print((void*)&s3);
	t_sphere s4 = sphere(vec3(1, 0.0, -1.0), 1, rgb(255,255,254));
	s4.print((void*)&s4);

	list[0] = (t_hittable*)(&s1);
	list[1] = (t_hittable*)(&s2);
	list[2] = (t_hittable*)(&s3);
	list[3] = (t_hittable*)(&s4);

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
