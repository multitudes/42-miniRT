#include "debug.h"
#include "libft.h"
#include "minirt.h"
#include "camera.h"
#include "vec3.h"
#include "hittable_list.h"
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

//keep y the same rotate around the y axis
t_point3 rotate_camera(t_point3 camera, double angle_degrees) {
    double angle_radians = degrees_to_radians(angle_degrees);

    // Keep the y coordinate the same
    double new_y = camera.y;

    // Calculate the new x and z using the rotation matrix
    double new_x = camera.x * cos(angle_radians) - camera.z * sin(angle_radians);
    double new_z = camera.x * sin(angle_radians) + camera.z * cos(angle_radians);

    // Return the new camera position
    return point3(new_x, new_y, new_z);
}


// Function to calculate the new camera direction vector (pointing toward the origin)
t_point3 calculate_direction(t_point3 camera_pos) {
    // Direction vector from the camera to the origin (0, 0, 0)
    t_point3 direction = point3(-camera_pos.x, -camera_pos.y, -camera_pos.z);

    // Normalize the direction vector
    return unit_vector(direction);
}

void	hook(void *param)
{
	mlx_t		*mlx;
	t_mrt		*mrt;

	mrt = (t_mrt *)param;
	mlx = mrt->mlx;
	if (mlx_is_key_down(mlx, MLX_KEY_ESCAPE))
		exit_gracefully(mlx);
	if (mlx_is_key_down(mlx, MLX_KEY_UP))
	{
		debug("UP key pressed");
	}
	if (mlx_is_key_down(mlx, MLX_KEY_DOWN))
		debug("DOWN key pressed");
	if (mlx_is_key_down(mlx, MLX_KEY_LEFT))
	{
		// mrt->cam.center = rotate_camera(mrt->cam.center, 5);
		// mrt->cam.direction = calculate_direction(mrt->cam.center);
		// mrt->renderscene(mrt, &(mrt->world), &(mrt->lights));
		debug("LEFT key pressed");
	}
	if (mlx_is_key_down(mlx, MLX_KEY_RIGHT)){
		// mrt->cam.center = rotate_camera(mrt->cam.center, -5);
		// mrt->cam.direction = calculate_direction(mrt->cam.center);
		// mrt->renderscene(mrt, &(mrt->world), &(mrt->lights));
		debug("RIGHT key pressed");
	}
	
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

	if (argc != 2)
	{
		write(2, "Error\nProgram expects an .rt file as input!\n", 44);
		return (EXIT_FAILURE);
	}
	ft_memset(&data, 0, sizeof(t_mrt));
	parse_input(argv[1], &data.objects);

	const t_hittablelist world = hittablelist(data.objects.hit_list, data.objects.hit_idx);


	// t_hittable *list_lights[1];
	// list_lights[0] = (t_hittable*)(&s6);
	const t_hittablelist lights = hittablelist(NULL, 0);

    debug("Start of minirt %s", "helllo !! ");

	if (!init_window(&data))
		return (EXIT_FAILURE);

	data.world = world;
	render(&data, &world, &lights);
	

    mlx_loop_hook(data.mlx, &hook, (void *)&data);

    mlx_loop(data.mlx);
    ft_printf("\nbyebye!\n");
    mlx_terminate(data.mlx);

    return (EXIT_SUCCESS);
}



// int main_old(int argc, char **argv)
// {
//     t_mrt data;
//     (void)argv;
// 	(void)argc;
// 	if (!init_data(&data))
//         return (1);

// 	// world
// 	t_hittable *list[4];

// 	t_sphere s1;
// 	sphere(&s1, vec3(0, 0, -1.2), 1, rgb(128,0,0));
// 	// s1.print((void*)&s1);
// 	t_sphere s2;
// 	sphere(&s2, vec3(0, -100.5, -1), 200, rgb(0,128,0));
// 	// s2.print((void*)&s2);
// 	t_sphere s3; 
// 	sphere(&s3, vec3(-1, 0.0, -1.0), 1, rgb(128,128,0));
// 	// s3.print((void*)&s3);
// 	t_sphere s4;
// 	sphere(&s4, vec3(1, 0.0, -1.0), 1, rgb(255,255,254));
// 	// s4.print((void*)&s4);

// 	list[0] = (t_hittable*)(&s1);
// 	list[1] = (t_hittable*)(&s2);
// 	list[2] = (t_hittable*)(&s3);
// 	list[3] = (t_hittable*)(&s4);

// 	const t_hittablelist world = hittablelist(list, 4);

//     debug("Start of minirt %s", "helllo !! ");
// 	if (!init_window(&data))
// 		return (EXIT_FAILURE);
	

// 	render(&data, &world, NULL);
	

//     mlx_loop_hook(data.mlx, &hook, (void *)&data);
//     mlx_loop(data.mlx);
//     mlx_terminate(data.mlx);
//     return (EXIT_SUCCESS);
// }
