#include "debug.h"
#include "libft.h"
#include "minirt.h"
#include "camera.h"
#include "vec3.h"
#include "ray.h"
#include "color.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"
#include "interval.h"
#include <MLX42/MLX42.h>
#include "utils.h"

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
    data->mlx = mlx_init(IMAGE_WIDTH, IMAGE_HEIGHT, WINDOW_TITLE, false);
	if (data->mlx == NULL)
		return (FALSE);
	data->image = mlx_new_image(data->mlx, IMAGE_WIDTH, IMAGE_HEIGHT);
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
    data->mlx = NULL;
    data->win_ptr = NULL;
    data->image = NULL;
    return (true);
}

double	deg_to_radians(double degrees)
{
	return (degrees / (180.0 / PI));
}

t_camera init_cam(t_point3 center, t_vec3 direction, double hfov) 
{
	(void)direction;
	t_camera cam;
	cam.samples_per_pixel = 10;
	cam.max_depth = 10;
	cam.aspect_ratio = (double)16.0/9.0;
	cam.image_width = IMAGE_WIDTH;
	cam.image_height = (int)(IMAGE_WIDTH/cam.aspect_ratio);
	cam.image_height = (cam.image_height < 1) ? 1 : cam.image_height;
	cam.center = center;
	cam.hfov = hfov;
	double viewport_height = 2.0;
	double viewport_width = viewport_height * ((double)IMAGE_WIDTH / IMAGE_HEIGHT);
	double focal_lenght = (1 / tan(degrees_to_radians(cam.hfov / 2))) * viewport_width / 2;

	t_vec3 viewport_u = vec3(viewport_width, 0, 0);
	t_vec3 viewport_v = vec3(0, -viewport_height, 0);

	cam.pixel_delta_u = vec3divscalar(viewport_u, IMAGE_WIDTH);
	cam.pixel_delta_v = vec3divscalar(viewport_v, IMAGE_HEIGHT);

	t_point3 part1 = vec3substr(cam.center, vec3(0, 0, focal_lenght));
	t_point3 part2 = vec3substr(part1, vec3divscalar(viewport_u, 2));
	t_point3 viewport_upper_left = vec3substr(part2, vec3divscalar(viewport_v, 2));

	cam.pixel00_loc = vec3add(viewport_upper_left, vec3divscalar(vec3add(cam.pixel_delta_u, cam.pixel_delta_v), 2));
    return cam;
}

int main(int argc, char **argv)
{
    t_mrt data;
    (void)argv;
	(void)argc;
	if (!init_data(&data))
        return (1);

	t_point3 center = point3(0,0,1);
	t_vec3 direction = vec3(0,0,-1);
	data.cam = init_cam(center, direction, 70);


	// world
	t_hittable *list[3];

	
	t_sphere s1 = sphere(vec3(0, 0, -1), 0.5);
	t_sphere s2 = sphere(vec3(0, -100.5, -1), 100);
	// t_sphere s3 = sphere(vec3(0, 0.1, -1.2), 0.5);
	list[0] = (t_hittable*)(&s1);
	list[1] = (t_hittable*)(&s2);
	// list[2] = (t_hittable*)(&s3);



	const t_hittablelist world = hittablelist(list, 2);




	
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



// void    write_colorbackground(t_mrt *data)
// {
//     int             x;
// 	int             y;
    
// 	x = 0;
// 	y = 0;
//     while (x < WINDOW_WIDTH)
//     {	
// 		y = 0;
//         while (y < WINDOW_HEIGHT)
//         {
//             write_color(data, x, y, 0xFF0000FF); // Drawing red color rgba
// 			y++;
//         }
// 		x++;
//     }
// }