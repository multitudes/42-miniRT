#include "debug.h"
#include "libft.h"
#include "minirt.h"
#include "camera.h"
#include "vec3.h"
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
		mrt->needs_render = true;
	}
	if (mlx_is_key_down(mlx, MLX_KEY_RIGHT)){
		// mrt->cam.center = rotate_camera(mrt->cam.center, -5);
		// mrt->cam.direction = calculate_direction(mrt->cam.center);
		// mrt->renderscene(mrt, &(mrt->world), &(mrt->lights));
		debug("RIGHT key pressed");
		mrt->needs_render = true;
	}
	if (mrt->needs_render)
	{
		render(mrt, &(mrt->world), &(mrt->lights));
		mrt->needs_render = false;
	}
}


int init_window(t_mrt *data)
{
    data->mlx = mlx_init(data->cam.image_width, data->cam.image_height, WINDOW_TITLE, true);
	if (data->mlx == NULL)
		return (FALSE);
	data->image = mlx_new_image(data->mlx, data->cam.image_width, data->cam.image_height);
	if (!(data->image))
	{
		mlx_close_window(data->mlx);
		ft_printf("%s\n", mlx_strerror(mlx_errno));
	}
	ft_memset(data->image->pixels, 0, data->cam.image_width * data->cam.image_height * BPP);
	if (mlx_image_to_window(data->mlx, data->image, 0, 0) == -1)
	{
		mlx_close_window(data->mlx);
		mlx_terminate(data->mlx);
		ft_printf("%s\n", mlx_strerror(mlx_errno));
    }
	// cursor, why not?
	mlx_create_std_cursor(MLX_CURSOR_CROSSHAIR);
	// mlx_texture_t* icon_image = mlx_xpm_file_to_image(data->mlx, "assets/42_icon.xpm", 0, 0);
	// mlx_set_icon(data->mlx, icon_image);
	debug("Window initialized");
    return (TRUE);
}

// int main_old(int argc, char **argv)
// {
//     t_mrt data;
//     (void)argv;
// 	(void)argc;

// 	ft_memset(&data, 0, sizeof(t_mrt));


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
	data->cam.image_width = new_width;
	data->cam.image_height = new_height;
	update_cam(&data->cam, new_width, new_height);
	mlx_resize_image(data->image, new_width, new_height);
	data->needs_render = true;
	// t_mrt		*data;

	// data = (t_mrt *)params;
	// data->cam.image_width = new_width;
	// data->cam.image_height = new_height;
	// update_cam(&data->cam, new_width, new_height);
	// mlx_image_t *old_image = data->image;
	// data->image = mlx_new_image(data->mlx, new_width, new_height);
	// if (!data->image)
	// {
	// 	if (old_image)
	// 		mlx_delete_image(data->mlx, old_image);
	// 	mlx_close_window(data->mlx);
	// 	mlx_terminate(data->mlx);
	// 	ft_printf("%s\n", mlx_strerror(mlx_errno));
	// 	exit(EXIT_FAILURE);
	// }
	// // render(data, &(data->world), &(data->lights));
	// if (mlx_image_to_window(data->mlx, data->image, 0, 0) == -1)
	// {
	// 	if (old_image)
	// 		mlx_delete_image(data->mlx, old_image);
	// 	mlx_close_window(data->mlx);
	// 	mlx_terminate(data->mlx);
	// 	ft_printf("%s\n", mlx_strerror(mlx_errno));
	// 	exit(EXIT_FAILURE);
	// }
	// if (old_image)
	// 	mlx_delete_image(data->mlx, old_image);

    // debug("Window resized to %d x %d", new_width, new_height);
	// // render(data, )
}

// main cylinder tests

int main(int argc, char **argv)
{
    t_mrt data;

	if (argc != 2)
	{
		write(2, "Error\nProgram expects an .rt file as input!\n", 44);
		return (EXIT_FAILURE);
	}
	ft_memset(&data, 0, sizeof(t_mrt));
	parse_input(argv[1], &data);


    debug("Start of minirt %s", "helllo !! ");

	if (!init_window(&data))
		return (EXIT_FAILURE);

	render(&data, &data.world, &data.lights);
    mlx_loop_hook(data.mlx, &hook, (void *)&data);

    mlx_loop(data.mlx);

    ft_printf("\nbyebye!\n");
    mlx_terminate(data.mlx);

    return (EXIT_SUCCESS);


}


// int main_mixtest(int argc, char **argv)
// {
//     t_mrt data;
//     (void)argv;
// 	(void)argc;

// 	/***************************** */
// 	/* 			camera 			   */	
// 	/***************************** */
// 	t_point3 center = point3(378, 378, -1800);
// 	t_vec3 direction = vec3(0,0,800);
// 	data.cam = init_cam(center, direction, 40);
// 	data.cam.print((void*)(&(data.cam)));

// 	/***************************** */
// 	/* 		ambient light		   */	
// 	/***************************** */
// 	t_ambient ambient_light = ambient(0.0, rgb(255,255,255));
// 	data.ambient_light = ambient_light;

// 	ambient_light.print((void*)&ambient_light);

// 	// world
// 	t_hittable *list[10];

// 	// red sphere
// 	t_sphere s1;
// 	sphere(&s1, vec3(190, 90, 190), 180, rgb(166, 13, 13));
// 	s1.print((void*)&s1);

// 	/***********************************/
// 	/* 			light        		   */
// 	/***********************************/
// 	t_diffuse_light difflight;
// 	t_solid_color difflight_color;
// 	solid_color_init(&difflight_color, color(40, 40, 40));
// 	diffuse_light_init(&difflight, (t_texture*)&difflight_color);
// 	t_diffuse_light difflight2;
// 	t_solid_color difflight_color2;
// 	solid_color_init(&difflight_color2, color(0, 0, 80));
// 	diffuse_light_init(&difflight2, (t_texture*)&difflight_color2);
// 	// t_quad s6 = quad(point3(343,554,332), vec3(-130,0,0), vec3(0,0,-105), (t_material*)&difflight);
// 	// t_sphere s6 = sphere_mat(point3( 343,554,332 ), 90, rgb(255,223 ,34 ), (t_material*)&difflight);
// 	t_sphere s2;
// 	sphere_mat(&s2, point3( 90,190,90 ), 30, (t_material*)&difflight2);
// 	// t_sphere s6 = sphere_mat(point3( 343,554,332), 90, (t_material*)&difflight);

// 	t_sphere s4;
// 	sphere(&s4, vec3(1, 0.0, -1.0), 1, rgb(255,255,254));


// 	t_sphere s5;
// 	sphere(&s5, vec3(0, 0, -1.2), 1, rgb(128,0,0));
// 	s1.print((void*)&s1);
// 	t_sphere s8;
// 	sphere(&s8, vec3(0, -100.5, -1), 200, rgb(0,128,0));
// 	s2.print((void*)&s2);
// 	t_sphere s7;
// 	sphere(&s7, vec3(-1, 0.0, -1.0), 1, rgb(128,128,0));


// 	list[0] = (t_hittable*)(&s1);
// 	// list[1] = (t_hittable*)(&s6);
// 	list[1] = (t_hittable*)(&s2);
// 	list[2] = (t_hittable*)(&s4);
// 	list[3] = (t_hittable*)(&s5);
// 	list[4] = (t_hittable*)(&s7);
// 	list[5] = (t_hittable*)(&s8);

// 	const t_hittablelist world = hittablelist(list, 6);

// 	t_hittable *list_lights[1];
// 	list_lights[0] = (t_hittable*)(&s2);
// 	// list_lights[1] = (t_hittable*)(&s6);
// 	const t_hittablelist lights = hittablelist(list_lights, 1);

//     debug("Start of minirt %s", "helllo !! ");
// 	if (!init_window(&data))
// 		return (EXIT_FAILURE);

// 	data.world.list = world.list;
// 	data.world.size = world.size;
// 	data.lights.list = lights.list;
// 	data.lights.size = lights.size;
// 	render(&data, &world, &lights);

//     mlx_loop_hook(data.mlx, &hook, (void *)&data);

//     mlx_loop(data.mlx);
//     ft_printf("\nbyebye!\n");
//     mlx_terminate(data.mlx);

//     return (EXIT_SUCCESS);
// }

