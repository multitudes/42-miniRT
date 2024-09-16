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
		mrt->cam.center = rotate_camera(mrt->cam.center, 5);
		mrt->cam.direction = calculate_direction(mrt->cam.center);
		mrt->renderscene(mrt, &(mrt->world), &(mrt->lights));
		debug("LEFT key pressed");
	}
	if (mlx_is_key_down(mlx, MLX_KEY_RIGHT)){
		mrt->cam.center = rotate_camera(mrt->cam.center, -5);
		mrt->cam.direction = calculate_direction(mrt->cam.center);
		mrt->renderscene(mrt, &(mrt->world), &(mrt->lights));
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

    return (true);
}

int main_mixtest(int argc, char **argv)
{
    t_mrt data;
    (void)argv;
	(void)argc;

	/***************************** */
	/* 			camera 			   */	
	/***************************** */
	t_point3 center = point3(378, 378, -1800);
	t_vec3 direction = vec3(0,0,800);
	data.cam = init_cam(center, direction, 40);
	data.cam.print((void*)(&(data.cam)));

	/***************************** */
	/* 		ambient light		   */	
	/***************************** */
	t_ambient ambient_light = ambient(0.0, rgb(255,255,255));
	data.ambient_light = ambient_light;

	ambient_light.print((void*)&ambient_light);

	// world
	t_hittable *list[10];

	// red sphere
	t_sphere s1 = sphere(vec3(190, 90, 190), 180, rgb(166, 13, 13));
	s1.print((void*)&s1);

	/***********************************/
	/* 			light        		   */
	/***********************************/
	t_diffuse_light difflight;
	t_solid_color difflight_color;
	solid_color_init(&difflight_color, color(40, 40, 40));
	diffuse_light_init(&difflight, (t_texture*)&difflight_color);
	t_diffuse_light difflight2;
	t_solid_color difflight_color2;
	solid_color_init(&difflight_color2, color(0, 0, 80));
	diffuse_light_init(&difflight2, (t_texture*)&difflight_color2);
	// t_quad s6 = quad(point3(343,554,332), vec3(-130,0,0), vec3(0,0,-105), (t_material*)&difflight);
	// t_sphere s6 = sphere_mat(point3( 343,554,332 ), 90, rgb(255,223 ,34 ), (t_material*)&difflight);
	t_sphere s2 = sphere_mat(point3( 90,190,90 ), 30, rgb(0,0,0), (t_material*)&difflight2);
	// t_sphere s6 = sphere_mat(point3( 343,554,332), 90, (t_material*)&difflight);

	t_sphere s4 = sphere(vec3(1, 0.0, -1.0), 1, rgb(255,255,254));


	t_sphere s5 = sphere(vec3(0, 0, -1.2), 1, rgb(128,0,0));
	s1.print((void*)&s1);
	t_sphere s8 = sphere(vec3(0, -100.5, -1), 200, rgb(0,128,0));
	s2.print((void*)&s2);
	t_sphere s7 = sphere(vec3(-1, 0.0, -1.0), 1, rgb(128,128,0));


	list[0] = (t_hittable*)(&s1);
	// list[1] = (t_hittable*)(&s6);
	list[1] = (t_hittable*)(&s2);
	list[2] = (t_hittable*)(&s4);
	list[3] = (t_hittable*)(&s5);
	list[4] = (t_hittable*)(&s7);
	list[5] = (t_hittable*)(&s8);

	const t_hittablelist world = hittablelist(list, 6);

	t_hittable *list_lights[1];
	list_lights[0] = (t_hittable*)(&s2);
	// list_lights[1] = (t_hittable*)(&s6);
	const t_hittablelist lights = hittablelist(list_lights, 1);

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



int main(int argc, char **argv)
{
    t_mrt data;
    (void)argv;
	(void)argc;

	/***************************** */
	/* 			camera 			   */	
	/***************************** */
	t_point3 center = point3(278, 278, -800);
	t_vec3 direction = vec3(0,0,800);
	data.cam = init_cam(center, direction, 60);
	data.cam.print((void*)(&(data.cam)));

	/***************************** */
	/* 		ambient light		   */	
	/***************************** */
	t_ambient ambient_light = ambient(1, rgb(110,110,110));
	data.cam.ambient_light = ambient_light;


	ambient_light.print((void*)&ambient_light);

	/*## RESOLUTION ##############
	# | width  | height | ######
	############################
	*/
	printf("R   %d     %d\n", data.cam.image_width, data.cam.image_height);

	/*## SAMPLING #####################
	# 	samples per pixel | bounces ###
	###################################
	*/
	printf("S   %d      %d\n", data.cam.samples_per_pixel, data.cam.max_depth);


	// world
	// ================================================== world ==================================================
	t_hittable *list[13];
	// ================================================== world ==================================================
	
	// red sphere
	// t_sphere s1 = sphere(vec3(190, 90, 190), 180, rgb(166, 13, 13));
	
	//red metallic 
	t_color albedo = color(0.8, 0.1, 0.1);
	double fuzz = 0.0;
	t_metal metal;
	metal_init(&metal, albedo, fuzz);
	// red metallic sphere
	t_sphere s1 = sphere_mat(point3( 90,190,90 ), 180, rgb(0,0,0), (t_material*)&metal);
	s1.print((void*)&s1);

	/***********************************/
	/* 			light        		   */
	/***********************************/
	t_diffuse_light difflight;
	t_solid_color difflight_color;
	solid_color_init(&difflight_color, color(40, 40, 40));
	diffuse_light_init(&difflight, (t_texture*)&difflight_color);

	// blue light
	t_diffuse_light difflight2;
	t_solid_color difflight_color2;
	solid_color_init(&difflight_color2, color(0, 0, 80));
	diffuse_light_init(&difflight2, (t_texture*)&difflight_color2);

	// quad as light
	t_quad s6 = quad(point3(343,554,332), vec3(-130,0,0), vec3(0,0,-105), (t_material*)&difflight);
	s6.print((void*)&s6);

	// t_sphere s6 = sphere_mat(point3( 343,554,332 ), 90, rgb(255,223 ,34 ), (t_material*)&difflight);
	// t_sphere s2 = sphere(vec3(0,250,-50), 120, rgb(16, 13, 166));
	t_sphere s2 = sphere_mat(point3( 0,250,-50 ), 120, rgb(0,0,0), (t_material*)&difflight2);
	s2.print((void*)&s2);
	// t_sphere s6 = sphere_mat(point3( 343,554,332), 90, (t_material*)&difflight);

	// adding another sphere
	// red sphere
	t_sphere s4 = sphere(vec3(400, 90, 190), 90, rgb(166, 13, 13));
	s4.print((void*)&s4);

	// add a quad just left of the s4 sphere
	t_quad s5 = quad_rgb(point3(300, 90, 100), vec3(50,0,100), vec3(0,100,50), rgb(166, 13, 13));
	s5.print((void*)&s5);

	// add a plane just below the s4 sphere
	t_plane s7 = plane(point3(400, 0, 190), vec3(0,1,0), rgb(166, 13, 13));
	s7.print((void*)&s7);

	t_disk s8;
	disk(&s8, point3(500, 90, 190), vec3(0,0,150), vec3(0,150,0), rgb(166, 53, 13));
	s8.print((void*)&s8);


// try with cube t_box box(t_point3 a, t_point3 b, t_material *mat)
	t_box s9;
	box(&s9, point3(600, 90, 190), point3(700, 190, 290), (t_material*)&metal);
	s9.print((void*)&s9);

	t_triangle s10 = triangle(point3(300, 101, 100), point3(200, 101, 290), point3(50, 101, 190), rgb(166, 103, 13));
	s10.print((void*)&s10);
	
	// checker texture sphere
	t_lambertian lambertian_material;
	t_checker_texture checker_texture1;
	t_solid_color even1;
	t_solid_color odd1;
	solid_color_init(&even1, color(0.2, 0.3, 0.1));
	solid_color_init(&odd1, color(0.9, 0.9, 0.9));
	checker_texture_init(&checker_texture1, 20.0, &even1, &odd1);
	lambertian_init_tex(&lambertian_material, (t_texture*)&(checker_texture1));
	t_sphere s11 = sphere_mat(point3(650, 300, 200), 100, rgb(0,0,0), (t_material*)&lambertian_material);

	printf("_______________\n");
	printf("size of t_triangle: %lu\n", sizeof(t_triangle));


	/***********************************/
	/* 			earth       		   */
	/***********************************/
	t_lambertian earth_surface;
	t_rtw_image img;
	init_rtw_image(&img,"rtw_image/earthmap.jpg");
	t_img_texture img_texture;
	img_texture_init(&img_texture, &img);
	lambertian_init_tex(&earth_surface, (t_texture*)&img_texture);
	t_sphere s12 = sphere_mat(point3(250, 100, -200), 100.0, rgb(0,0,0) ,(t_material*)&earth_surface);
	s12.print((void*)&s12);
	
	t_cylinder s13;
	cylinder_u(&s13, point3(350, 100, -400), vec3(0,1,0), 100, 100, rgb(166, 103, 13));
	s13.print((void*)&s13);

	list[0] = (t_hittable*)(&s1);
	list[1] = (t_hittable*)(&s6);
	list[2] = (t_hittable*)(&s2);
	list[3] = (t_hittable*)(&s4);
	list[4] = (t_hittable*)(&s5);	
	list[5] = (t_hittable*)(&s7);
	list[6] = (t_hittable*)(&s8);
	list[7] = (t_hittable*)(&s9);
	list[8] = (t_hittable*)(&s10);
	list[9] = (t_hittable*)(&s11);
	list[10] = (t_hittable*)(&s12);
	list[11] = (t_hittable*)(&s13);

	const t_hittablelist world = hittablelist(list, 12);

	t_hittable *list_lights[2];

	t_empty_material empty_material;
	t_material *no_material = (t_material*)&empty_material;
	t_quad l6 = quad(point3(343,554,332), vec3(-130,0,0), vec3(0,0,-105), (t_material*)&no_material);

	// t_sphere s6 = sphere_mat(point3( 343,554,332 ), 90, rgb(255,223 ,34 ), (t_material*)&difflight);
	t_sphere l2 = sphere_mat(point3( 0,250,-50 ), 120, rgb(0,0,0),(t_material*)&no_material);
//0,250,-50 ), 120
	list_lights[0] = (t_hittable*)(&l6);
	list_lights[1] = (t_hittable*)(&l2);
	const t_hittablelist lights = hittablelist(list_lights, 2);

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


int main_earth(int argc, char **argv)
{
    t_mrt data;
    (void)argv;
	(void)argc;
	if (!init_data(&data))
        return (1);
	/***************************** */
	/* 			camera 			   */	
	/***************************** */
	t_point3 center = point3(0,4,4);
	t_vec3 direction = vec3(0,-2,-2);
	data.cam = init_cam(center, direction, 50);
	data.cam.print((void*)(&(data.cam)));

	/*## RESOLUTION ##############
	# | width  | height | ######
	############################
	*/
	printf("R   %d     %d\n", data.cam.image_width, data.cam.image_height);

	/*## SAMPLING #####################
	# 	samples per pixel | bounces ###
	###################################
	*/
	printf("S   %d      %d\n", data.cam.samples_per_pixel, data.cam.max_depth);



	/***************************** */
	/* 		ambient light		   */	
	/***************************** */
	t_ambient ambient_light = ambient(0.0, rgb(255,255,255));
	data.ambient_light = ambient_light;

	ambient_light.print((void*)&ambient_light);

	// world
	t_hittable *list[7];

	// red sphere
	t_sphere s1 = sphere(vec3(0, 0, -2.0), 1, rgb(128,0,0));
	s1.print((void*)&s1);

	// t_sphere s2 = sphere(vec3(0, -100.5, -1), 200, rgb(0,128,0));
	// s2.print((void*)&s2);
	
	// checker texture sphere
	t_lambertian lambertian_material;
	t_checker_texture checker_texture1;
	t_solid_color even1;
	t_solid_color odd1;
	solid_color_init(&even1, color(0.2, 0.3, 0.1));
	solid_color_init(&odd1, color(0.9, 0.9, 0.9));
	checker_texture_init(&checker_texture1, 0.31, &even1, &odd1);
	lambertian_init_tex(&lambertian_material, (t_texture*)&(checker_texture1));
	t_sphere s2 = sphere_mat(point3(0, -500.5, -1), 1000, rgb(0,0,0), (t_material*)&lambertian_material);
	
	// yellow sphere
	t_sphere s3 = sphere(vec3(-1, 0.0, -2.0), 1, rgb(255,219,0));
	s3.print((void*)&s3);
	// white
	t_sphere s4 = sphere(vec3(1, 0.0, -2.0), 1, rgb(255,255,254));
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

	// /***********************************/
	// /* 			mars        		   */
	// /***********************************/

	// t_lambertian mars_surface;
	// t_rtw_image img2;
	// init_rtw_image(&img2,"rtw_image/jupiter.jpg");
	// t_img_texture img_texture2;
	// img_texture_init(&img_texture2, &img2);
	// lambertian_init_tex(&mars_surface, (t_texture*)&img_texture2);
	// t_sphere s6 = sphere_mat(point3(-1, 0, 0), 2.0, rgb(0,0,0) ,(t_material*)&mars_surface);

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

	/***********************************/
	/* 			light        		   */
	/***********************************/
	t_diffuse_light difflight;
	t_solid_color difflight_color;
	solid_color_init(&difflight_color, color(20, 20, 20));
	diffuse_light_init(&difflight, (t_texture*)&difflight_color);
	// t_sphere s6 = sphere_mat(point3(5, 0, 0), 5.0, rgb(255,223 ,34 ), (t_material*)&difflight);
	 t_quad s6 = quad(point3(50, 20, 20), vec3(-30,0,0), vec3(0,0,-30), (t_material*)&difflight);
	
	list[0] = (t_hittable*)(&s1);
	list[1] = (t_hittable*)(&s2);
	list[2] = (t_hittable*)(&s3);
	list[3] = (t_hittable*)(&s4);
	list[4] = (t_hittable*)(&s5);
	list[5] = (t_hittable*)(&s6);

	const t_hittablelist world = hittablelist(list, 6);


	t_hittable *list_lights[1];
	list_lights[0] = (t_hittable*)(&s6);
	const t_hittablelist lights = hittablelist(list_lights, 1);

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



int main_old(int argc, char **argv)
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
	

	render(&data, &world, NULL);
	

    mlx_loop_hook(data.mlx, &hook, (void *)&data);

    mlx_loop(data.mlx);
    ft_printf("\nbyebye!\n");
    mlx_terminate(data.mlx);

    return (EXIT_SUCCESS);
}
