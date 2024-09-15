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
		// mrt->renderscene(mrt, &(mrt->world));
		debug("LEFT key pressed");
	}
	if (mlx_is_key_down(mlx, MLX_KEY_RIGHT)){
		// mrt->cam.center = rotate_camera(mrt->cam.center, -5);
		// mrt->cam.direction = calculate_direction(mrt->cam.center);
		// mrt->renderscene(mrt, &(mrt->world));
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


void	print_vector(t_vec3 vec, char *msg)
{
	printf("%s: ", msg);
	printf("%f,%f,%f\n", vec.x, vec.y, vec.z);
}

int main(int argc, char **argv)
{
    t_mrt data;
    (void)argv;
	(void)argc;

	// check if argc is 2
	ft_memset(&data, 0, sizeof(t_mrt));
	parse_input("scenes/first.rt", &data.objects);



	const t_hittablelist world = hittablelist(data.objects.hit_list, data.objects.hit_idx);
	printf("hittable list size: %i\n", world.size);

	for (int i = 0; i < world.size; ++i)
		printf("hittable list member %i has hit function %p\n", i, world.list[i]->hit);

	printf("\nthe camera\n");
	print_vector(data.objects.camera.center, "center");
	print_vector(data.objects.camera.direction, "direction");
	printf("hfov: %f\n", data.objects.camera.hfov);


    debug("Start of minirt %s", "helllo !! ");

	if (!init_window(&data))
		return (EXIT_FAILURE);

	render(&data, &world);

    mlx_loop_hook(data.mlx, &hook, (void *)&data);
    mlx_loop(data.mlx);
    mlx_terminate(data.mlx);
    return (EXIT_SUCCESS);
}
