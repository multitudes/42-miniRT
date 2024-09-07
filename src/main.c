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
		mrt->cam.center = rotate_camera(mrt->cam.center, 5);
		mrt->cam.direction = calculate_direction(mrt->cam.center);
		mrt->renderscene(mrt, &(mrt->world));
		debug("LEFT key pressed");
	}
	if (mlx_is_key_down(mlx, MLX_KEY_RIGHT)){
		mrt->cam.center = rotate_camera(mrt->cam.center, -5);
		mrt->cam.direction = calculate_direction(mrt->cam.center);
		mrt->renderscene(mrt, &(mrt->world));
		debug("RIGHT key pressed");
	}
	
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
	data->renderscene = render;

	/***************************** */
	/* 			camera 			   */	
	/***************************** */
	t_point3 center = point3(-2,2,1);
	t_vec3 direction = vec3(2,-2,-2);
	data->cam = init_cam(center, direction, 120);
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
	t_hittable *list[6];

	t_sphere s1 = sphere(vec3(0, 0, -1.2), 1, rgb(128,0,0));
	s1.print((void*)&s1);

	// t_sphere s2 = sphere(vec3(0, -100.5, -1), 200, rgb(0,128,0));
	// s2.print((void*)&s2);
	
	t_lambertian lambertian_material;
	t_checker_texture checker_texture1;
	t_solid_color even1;
	t_solid_color odd1;
	solid_color_init(&even1, color(0.2, 0.3, 0.1));
	solid_color_init(&odd1, color(0.9, 0.9, 0.9));
	checker_texture_init(&checker_texture1, 0.31, &even1, &odd1);
	lambertian_init_tex(&lambertian_material, (t_texture*)&(checker_texture1));
	t_sphere s2 = sphere_mat(point3(0, -100.5, -1), 200, rgb(0,0,0), (t_material*)&lambertian_material);


	
	
	t_sphere s3 = sphere(vec3(-1, 0.0, -1.0), 1, rgb(128,128,0));
	s3.print((void*)&s3);
	t_sphere s4 = sphere(vec3(1, 0.0, -1.0), 1, rgb(255,255,254));
	s4.print((void*)&s4);

	/***********************************/
	/* 			earth       		   */
	/***********************************/
	t_lambertian earth_surface;
	t_rtw_image img;
	init_rtw_image(&img,"rtw_image/earthmap.jpg");
	t_img_texture img_texture;
	img_texture_init(&img_texture, &img);
	lambertian_init_tex(&earth_surface, (t_texture*)&img_texture);
	t_sphere s5 = sphere_mat(point3(0.0, 0, 0), 2.0, rgb(0,0,0) ,(t_material*)&earth_surface);

	/***********************************/
	/* 			mars        		   */
	/***********************************/

	t_lambertian mars_surface;
	t_rtw_image img2;
	init_rtw_image(&img2,"rtw_image/jupiter.jpg");
	t_img_texture img_texture2;
	img_texture_init(&img_texture2, &img2);
	lambertian_init_tex(&mars_surface, (t_texture*)&img_texture2);
	t_sphere s6 = sphere_mat(point3(-1, 0, 0), 2.0, rgb(0,0,0) ,(t_material*)&mars_surface);

	/***********************************/
	/* 			checker        		   */
	/***********************************/
	// t_lambertian lambertian_material;
	// t_checker_texture checker_texture1;
	// t_solid_color even1;
	// t_solid_color odd1;
	// solid_color_init(&even1, color(0.2, 0.3, 0.1));
	// solid_color_init(&odd1, color(0.9, 0.9, 0.9));
	// checker_texture_init(&checker_texture1, 0.31, &even1, &odd1);
	// lambertian_init_tex(&lambertian_material, (t_texture*)&(checker_texture1));
	// t_sphere s6 = sphere_mat(point3(-1, 0, 0), 2.0, rgb(0,0,0), (t_material*)&lambertian_material);


	list[0] = (t_hittable*)(&s1);
	list[1] = (t_hittable*)(&s2);
	list[2] = (t_hittable*)(&s3);
	list[3] = (t_hittable*)(&s4);
	list[4] = (t_hittable*)(&s5);
	list[5] = (t_hittable*)(&s6);

	const t_hittablelist world = hittablelist(list, 6);

    debug("Start of minirt %s", "helllo !! ");
	if (!init_window(&data))
		return (EXIT_FAILURE);

	data.world = world;
	render(&data, &world);
	

    mlx_loop_hook(data.mlx, &hook, (void *)&data);

    mlx_loop(data.mlx);
    ft_printf("\nbyebye!\n");
    mlx_terminate(data.mlx);

    return (EXIT_SUCCESS);
}
