/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 17:31:01 by lbrusa            #+#    #+#             */
/*   Updated: 2024/10/03 15:56:57 by lbrusa           ###   ########.fr       */
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

int main_checkerfloors();
int main_earth_nolight_pinkambient();
int main_blue_red();
int main_lights_three_lambertian();
int main_cyl_uncapped_disk();
int main_camera_center();
int render_from_file(char *filename);
int init_window(t_mrt *data);
bool init_data(t_mrt *data);
int main_plane_orientation();
int main_quad();
int main_showcase_all();


int main(int argc, char **argv)
{

    (void)argv;
	(void)argc;

	if (argc > 1 && argc < 3)
	{
		render_from_file(argv[1]);
	}
	else 
	{	
		int scene = 6;

		switch (scene)
		{
		case 1:
			main_lights_three_lambertian();
			break;
		case 2:
			main_earth_nolight_pinkambient();
			break;
		case 3:
			main_blue_red();
			break;
		case 4:
			main_checkerfloors();
			break;
		case 5:
			main_lights_three_lambertian();
			break;
		case 6:
			main_cyl_uncapped_disk();
			break;
		case 7:
			main_camera_center();
			break;
		case 8:
			main_plane_orientation();
			break;
		case 9:
			main_quad();
			break;
		case 10:
			main_showcase_all();
			break;
		default:
			break;
		}
		return (EXIT_SUCCESS);
	}
}

int main_showcase_all()
{
	
	return 0;
}

//debug 
// qd    300,10,200     100,10,100 200,10,200         166,53,13  
int main_quad()
{

t_mrt data;
	// world
	t_hittable *list[7];

	if (!init_data(&data))
        return (1);
	
	/***************************** */
	/* 			camera 			   */	
	/***************************** */
	// t_point3 center = point3(0, 0, 0);
	//343,212,-490    
	t_point3 center = point3(-100, 212, -490);
	t_vec3 direction = vec3(0, 0, 1);
	init_cam(&data.cam, center, direction, 60);
	data.cam.print((void*)(&(data.cam)));

	/***************************** */
	/* 		ambient light		   */	
	/***************************** */
	ambient(&data.cam.ambient, 1, rgb(110,110,110));
	data.cam.ambient.print((void*)&data.cam.ambient);

	// red plane
	// t_lambertian lambertian_material;
	// lambertian_init(&lambertian_material, color(200, 0, 0));
	// pl	0,100,0		0,1,0 	111,111,111
	t_plane s0;
	// t_metal metal_material;
	// metal_init(&metal_material, color_to_rgb(color(0.5, 0, 0)), 0.3);
	// plane_mat(&s0, point3(0, 100, 0), vec3(0,1,0), (t_material*)&metal_material);
	plane(&s0, point3(0, -100, 0), vec3(0,-1,0), rgb(200,0,0));
	s0.print((void*)&s0); 
	
	// qd    300,10,200     100,10,100 200,10,200         166,53,13  
	// -130,0,0    0,0,-105 
	t_quad s1;
	quad_rgb(&s1, point3(0,100,0), vec3(-130,0,0), vec3(0,0,-105), rgb(0,255,13));
	s1.print((void*)&s1);

	list[0] = (t_hittable*)(&s0);
	list[1] = (t_hittable*)(&s1);

	const t_hittablelist world = hittablelist(list, 2);

	t_hittable *list_lights[1];

	t_empty_material empty_material;
	t_material *no_material = (t_material*)&empty_material;
	t_quad l6;
	quad_mat(&l6, point3(0.1,0.1,0.1), vec3(0.1,1,0.1), vec3(1,0.1,0.1), (t_material*)&no_material);

	// t_sphere l6;
	// sphere_mat(&l6, point3(250, 100, -200), 200.0, (t_material*)&no_material);

	// l6.print((void*)&l6);

	list_lights[0] = (t_hittable *) &l6;
	// list_lights[0] = (t_hittable*)(&l6);
	const t_hittablelist lights = hittablelist(list_lights, 1);
    debug("Start of minirt %s", "helllo !! ");
	if (!init_window(&data))
		return (EXIT_FAILURE);

	data.world = world;
	data.lights = lights;
	render(&data, &world, &lights);
	
    mlx_loop_hook(data.mlx, &hook, (void *)&data);
	mlx_resize_hook(data.mlx, &_resize_hook, (void *)&data);
    mlx_loop(data.mlx);
    ft_printf("\nbyebye!\n");
    mlx_terminate(data.mlx);

    return (EXIT_SUCCESS);

}

int main_plane_orientation()
{
	t_mrt data;
	// world
	t_hittable *list[7];

	if (!init_data(&data))
        return (1);
	
	/***************************** */
	/* 			camera 			   */	
	/***************************** */
	t_point3 center = point3(0, 0, 0);
	t_vec3 direction = vec3(0, 0, 200);
	init_cam(&data.cam, center, direction, 60);
	data.cam.print((void*)(&(data.cam)));

	/***************************** */
	/* 		ambient light		   */	
	/***************************** */
	ambient(&data.cam.ambient, 1, rgb(110,110,110));
	data.cam.ambient.print((void*)&data.cam.ambient);

	// red plane
	// t_lambertian lambertian_material;
	// lambertian_init(&lambertian_material, color(200, 0, 0));
	// pl	0,100,0		0,1,0 	111,111,111
	t_plane s0;
	t_metal metal_material;
	metal_init(&metal_material, color_to_rgb(color(0.5, 0, 0)), 0.3);
	plane_mat(&s0, point3(0, 100, 0), vec3(0,1,0), (t_material*)&metal_material);
	// plane(&s0, point3(0, -100, 0), vec3(0,-1,0), rgb(200,0,0));
	s0.print((void*)&s0); 
	
	t_sphere s1;
	sphere(&s1, point3(0,0,600), 400, rgb(0,0,200));

	list[0] = (t_hittable*)(&s0);
	list[1] = (t_hittable*)(&s1);

	const t_hittablelist world = hittablelist(list, 2);

	t_hittable *list_lights[1];

	t_empty_material empty_material;
	t_material *no_material = (t_material*)&empty_material;
	t_quad l6;
	quad_mat(&l6, point3(0.1,0.1,0.1), vec3(0.1,1,0.1), vec3(1,0.1,0.1), (t_material*)&no_material);

	// t_sphere l6;
	// sphere_mat(&l6, point3(250, 100, -200), 200.0, (t_material*)&no_material);

	l6.print((void*)&l6);

	list_lights[0] = (t_hittable *) &l6;
	// list_lights[0] = (t_hittable*)(&l6);
	const t_hittablelist lights = hittablelist(list_lights, 1);
    debug("Start of minirt %s", "helllo !! ");
	if (!init_window(&data))
		return (EXIT_FAILURE);

	data.world = world;
	data.lights = lights;
	render(&data, &world, &lights);
	
    mlx_loop_hook(data.mlx, &hook, (void *)&data);
	mlx_resize_hook(data.mlx, &_resize_hook, (void *)&data);
    mlx_loop(data.mlx);
    ft_printf("\nbyebye!\n");
    mlx_terminate(data.mlx);

    return (EXIT_SUCCESS);

}
int	main_camera_center()
{
    t_mrt data;
	init_data(&data);

	/***************************** */
	/* 			camera 			   */
	/***************************** */
	t_point3 center = point3(210, 330, -130);
	t_vec3 direction = vec3(0,0,1);
	init_cam(&data.cam, center, direction, 120);
	data.cam.print((void*)(&(data.cam)));

	/***************************** */
	/* 		ambient light		   */
	/***************************** */
	ambient(&data.cam.ambient, 0.3, rgb(110,100,100));
	data.cam.ambient.print((void*)&data.cam.ambient);

	/***********************************/
	/* 			light        		   */
	/***********************************/
	t_diffuse_light difflight;
	t_solid_color difflight_color;
	solid_color_init(&difflight_color, color(100, 100, 100));
	diffuse_light_init(&difflight, (t_texture*)&difflight_color);

	// world ================================================== world ==================================================
	t_hittable *list[10];

	// red sphere
	t_sphere s1;
	sphere(&s1, vec3(190, 90, 190), 180, rgb(166, 13, 13));
	s1.print((void*)&s1);

// light top
	t_quad s6;
	quad_mat(&s6, point3(343,554,332), vec3(-200,0,0), vec3(0,0,-200), (t_material*)&difflight);
	s6.print((void*)&s6);

	list[0] = (t_hittable*)(&s1); // red sphere
	list[1] = (t_hittable*)(&s6);  // light quad

	const t_hittablelist world = hittablelist(list, 2);

	t_empty_material no_material;
	empty_material_init(&no_material);

	t_quad l6;
	quad_mat(&l6, point3(343,554,332), vec3(-200,0,0), vec3(0,0,-200), (t_material*)&no_material);
	t_hittable *list_lights[1];
	list_lights[0] = (t_hittable*)(&l6);
	const t_hittablelist lights = hittablelist(list_lights, 1);
    debug("Start of minirt %s", "helllo !! ");
	if (!init_window(&data))
		return (EXIT_FAILURE);
	data.world= world;
	data.lights = lights;
	render(&data, &world, &lights);
	mlx_resize_hook(data.mlx, &_resize_hook, (void *)&data);
	// mlx_key_hook(data.mlx, key_callback, &data);
    mlx_loop_hook(data.mlx, &hook, (void *)&data);
    mlx_loop(data.mlx);
    ft_printf("\nbyebye!\n");
    mlx_terminate(data.mlx);
    return (EXIT_SUCCESS);
}


int init_window(t_mrt *data)
{

    data->mlx = mlx_init(data->cam.image_width, data->cam.image_height, WINDOW_TITLE, true);
	if (data->mlx == NULL)
		return (FALSE);
	data->image = mlx_new_image(data->mlx, (uint32_t)data->cam.image_width, (uint32_t)data->cam.image_height);
	if (!(data->image))
	{
		mlx_close_window(data->mlx);
		ft_printf("%s\n", mlx_strerror(mlx_errno));
	}
	ft_memset(data->image->pixels, (int)0, (size_t)data->cam.image_width * (size_t)data->cam.image_height * (size_t)BPP);
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
	data->cam.image_width = new_width;
	data->cam.image_height = new_height;
	update_cam_resize(&data->cam, new_width, new_height);
	mlx_resize_image(data->image, (uint32_t)new_width, (uint32_t)new_height);
	data->needs_render = true;
}

int render_from_file(char *filename)
{
    t_mrt data;

	// if (argc != 2)
	// {
	// 	write(2, "Error\nProgram expects an .rt file as input!\n", 44);
	// 	return (EXIT_FAILURE);
	// }
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
// 	if (!init_data(&data))
//         return (1);

// 	// world
// 	t_hittable *list[4];

// 	t_sphere s1;
// 	sphere(&s1, vec3(0, 0, -1.2), 1, rgb(128,0,0));
// 	s1.print((void*)&s1);
// 	t_sphere s2;
// 	sphere(&s2, vec3(0, -100.5, -1), 200, rgb(0,128,0));
// 	s2.print((void*)&s2);
// 	t_sphere s3;
// 	sphere(&s3, vec3(-1, 0.0, -1.0), 1, rgb(128,128,0));
// 	s3.print((void*)&s3);
// 	t_sphere s4;
// 	sphere(&s4, vec3(1, 0.0, -1.0), 1, rgb(255,255,254));
// 	s4.print((void*)&s4);

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

//     ft_printf("\nbyebye!\n");
//     mlx_terminate(data.mlx);

//     return (EXIT_SUCCESS);
// }

/*





C       278,278,-800            0,0,1         70
A       0               255,255,255
sp      190,90,190              180             1,0,130
l	   343,554,332              0.7           40,40,40
l	   343,354,332              0.9          0,0,255    200




*/
int main_blue_red()
{
	t_mrt data;

	/***************************** */
	/* 			camera 			   */
	/***************************** */
	t_point3 center = point3(214, 265, -289);
	t_vec3 direction = vec3(0,0,1);
 	init_cam(&data.cam, center, direction, 70);
	data.cam.print((void*)(&(data.cam)));

	/***************************** */
	/* 		ambient light		   */
	/***************************** */
	ambient(&data.cam.ambient,0.0, rgb(110,110,110));
	data.cam.ambient.print((void*)&	data.cam.ambient);

	t_diffuse_light difflight;
	t_solid_color difflight_color;
	solid_color_init(&difflight_color, color(255, 255, 255));
	diffuse_light_init(&difflight, (t_texture*)&difflight_color);

	// blue light
	t_diffuse_light blue;
	t_solid_color diff_lightblue;
	solid_color_init(&diff_lightblue, color(1, 1, 255));
	diffuse_light_init(&blue, (t_texture*)&diff_lightblue);

	// world
	// ================================================== world ==================================================
	t_hittable *list[3];

	// "red" sphere
	// t_sphere s1;
	// sphere(&s1, vec3(190, 90, 190), 180, rgb(200, 0, 0));
	t_metal metal;
	metal_init(&metal, color_to_rgb(color(0.8, 0, 0)), 0.3);

	// red metallic sphere
	t_sphere s1;
	sphere_mat(&s1, vec3(190, 90, 190), 180, (t_material*)&metal);
	s1.print((void*)&s1);

	// red non metallic sphere
	t_sphere s2;
	sphere(&s2, vec3(90, 190, 90), 60, rgb(200, 200, 200));
	s2.print((void*)&s2);

// as light also
	// t_sphere s2;
	// sphere_mat(&s2, point3( 343,554,332 ), 100, (t_material*)&difflight);
	// blue
	// t_sphere s3;
	// sphere_mat(&s3, point3( 343,354,332 ), 200, (t_material*)&difflight);
	t_quad s3;
	quad_mat(&s3, point3(343,554,332), vec3(-130,0,0), vec3(0,0,-105), (t_material*)&blue);
	s3.print((void*)&s3);
	// t_disk s3;
	// disk_mat(&s3, point3(343,554,332), vec3(0,-1,0), 200, (t_material*)&difflight);

	// t_disk s3;
	// disk_mat(&s3, point3(343,554,332), vec3(0,-1,0), 200, (t_material*)&blue);
	// s3.print((void*)&s3);

	// t_disk s3;
	// disk(&s3, point3(343,554,332), vec3(0,1,0), 200, rgb(255, 255, 255));

	list[0] = (t_hittable*)(&s1);
	list[1] = (t_hittable*)(&s2);
	list[2] = (t_hittable*)(&s3);

	const t_hittablelist world = hittablelist(list, 3);

	t_hittable *list_lights[1];

	t_empty_material empty_material;
	empty_material_init(&empty_material);
	t_material *no_material = (t_material*)&empty_material;

	// t_sphere l1;
	// sphere_mat(&l1, point3( 343,554,332 ), 100, (t_material*)&no_material);
	// t_sphere l2;
	// sphere_mat(&l2, point3( 343,354,132 ), 200, (t_material*)&no_material);
	// t_quad l2;
	// quad_mat(&l2, point3(343,554,332), vec3(-130,0,0), vec3(0,0,-105), (t_material*)&no_material);
	// t_disk l2;
	// disk_mat(&l2, point3(343,554,332), vec3(0,0,1), 10, (t_material*)&no_material);
	t_quad l2;
	quad_mat(&l2, point3(343,554,332), vec3(-130,0,0), vec3(0,0,-105), (t_material*)&no_material);

	list_lights[0] = (t_hittable*)(&s3);
	// list_lights[1] = (t_hittable*)(&l2);

	const t_hittablelist lights = hittablelist(list_lights, 1);


    debug("Start of minirt %s", "helllo !! ");
	if (!init_window(&data))
		return (EXIT_FAILURE);

	data.world = world;
	data.lights = lights;
	render(&data, &world, &lights);

	mlx_resize_hook(data.mlx, &_resize_hook, (void *)&data);

    mlx_loop_hook(data.mlx, &hook, (void *)&data);

    mlx_loop(data.mlx);
    ft_printf("\nbyebye!\n");
    mlx_terminate(data.mlx);

    return (EXIT_SUCCESS);


}



int main_lights_three_lambertian()
{
    t_mrt data;
	if (!init_data(&data))
        return (1);


	/***************************** */
	/* 			camera 			   */
	/***************************** */
	t_point3 center = point3( -800.000000, -234, -250.000000);
	t_vec3 direction = vec3(1,1,1);
	init_cam(&data.cam, center, direction, 90);
	data.cam.print((void*)(&(data.cam)));

	/***************************** */
	/* 		ambient light		   */
	/***************************** */
	ambient(&data.cam.ambient, 1, rgb(110,100,100));
	data.cam.ambient.print((void*)&data.cam.ambient);

	/***********************************/
	/* 			light        		   */
	/***********************************/
	t_diffuse_light difflight;
	t_solid_color difflight_color;
	solid_color_init(&difflight_color, color(100, 100, 100));
	diffuse_light_init(&difflight, (t_texture*)&difflight_color);

	// world ================================================== world ==================================================
	t_hittable *list[10];

	// red sphere
	t_sphere s1;
	sphere(&s1, vec3(190, 90, 190), 180, rgb(166, 13, 13));
	s1.print((void*)&s1);

// light top
	t_quad s6;
	quad_mat(&s6, point3(343,554,332), vec3(-200,0,0), vec3(0,0,-200), (t_material*)&difflight);
	s6.print((void*)&s6);

	// // t_sphere s6 = sphere_mat(point3( 343,554,332 ), 90, rgb(255,223 ,34 ), (t_material*)&difflight);
	// t_sphere s2;
	// sphere_mat(&s2, point3( 90,190,90 ), 60, (t_material*)&blue);
	// // t_sphere s6 = sphere_mat(point3( 343,554,332), 90, (t_material*)&difflight);

	t_sphere s4;
	sphere(&s4, vec3(350, 350, 250), 160, rgb(10,0,0));
	s4.print((void*)&s4);

	t_sphere s5;
	sphere(&s5, vec3(100, 0, -101.2), 350, rgb(128,0,0));
	s5.print((void*)&s5);

	t_sphere s8;
	sphere(&s8, vec3(0, -100.5, -1), 200, rgb(0,128,0));
	s8.print((void*)&s8);

	t_sphere s7;
	sphere(&s7, vec3(-1, 0.0, -1.0), 150, rgb(128,128,0));
	s7.print((void*)&s7);

	list[0] = (t_hittable*)(&s1); // red sphere
	list[1] = (t_hittable*)(&s6);  // light quad
	list[2] = (t_hittable*)(&s4);
	list[3] = (t_hittable*)(&s5);
	list[4] = (t_hittable*)(&s7);
	list[5] = (t_hittable*)(&s8);

	const t_hittablelist world = hittablelist(list, 6);

	t_empty_material empty_material;
	empty_material_init(&empty_material);

	t_sphere l2;
	sphere_mat(&l2, point3( 343,554,332 ), 10, (t_material*)&empty_material);

	// t_quad s6;
	// quad_mat(&s6, point3(343,554,332), vec3(-130,0,0), vec3(0,0,-105), (t_material*)&difflight);
	t_quad l6;
	quad_mat(&l6, point3(343,554,332), vec3(-200,0,0), vec3(0,0,-200), (t_material*)&empty_material);

	t_hittable *list_lights[2];
	list_lights[0] = (t_hittable*)(&l6);
	list_lights[1] = (t_hittable*)(&l2);
	const t_hittablelist lights = hittablelist(list_lights, 1);

    debug("Start of minirt %s", "helllo !! ");
	if (!init_window(&data))
		return (EXIT_FAILURE);

	data.world= world;
	data.lights = lights;

	render(&data, &world, &lights);

	mlx_resize_hook(data.mlx, &_resize_hook, (void *)&data);
    mlx_loop_hook(data.mlx, &hook, (void *)&data);
    mlx_loop(data.mlx);
    ft_printf("\nbyebye!\n");
    mlx_terminate(data.mlx);

    return (EXIT_SUCCESS);
}

// plane test
int main_()
{

	return (0);

}

int main_cyl_uncapped_disk()
{
    t_mrt data;


    /***************************** */
    /*          camera            */
    /***************************** */
    t_point3 center = point3(-5, 80, 291);
    t_vec3 direction = vec3(0, 0, -400);
    init_cam(&data.cam, center, direction, 60);
    data.cam.print((void*)(&(data.cam)));

    /***************************** */
    /*       ambient light        */
    /***************************** */
    ambient(&data.cam.ambient, 1, rgb(255, 255, 255));

    // world
    // ================================================== world ==================================================
    t_hittable *list[2];

    // Adjusted triangle coordinates
    t_triangle d0;
    triangle(&d0, point3(0, 0, 100), point3(-50, 0, 0), point3(50, 0, 150), rgb(166, 103, 13));
    d0.print((void*)&d0);

    t_rgb albedo = color_to_rgb(color(0.1, 0.8, 0.1));
    double fuzz = 0.0;
    t_metal metal;
    metal_init(&metal, albedo, fuzz);

    t_cylinder c0;
    cylinder_mat_uncapped(&c0, point3(0, 0, 0), vec3(0, 1, 0), 200, 10, (t_material*)&metal);
    c0.print((void*)&c0);

    list[0] = (t_hittable*)(&d0);
    list[1] = (t_hittable*)(&c0);

    const t_hittablelist world = hittablelist(list, 2);

    t_hittable *list_lights[2];
    t_empty_material empty_material;
    t_material *no_material = (t_material*)&empty_material;
    t_quad l6;
    quad_mat(&l6, point3(343, 554, 332), vec3(-130, 0, 0), vec3(0, 0, -105), (t_material*)&no_material);

    t_sphere l2;
    sphere_mat(&l2, point3(0, 250, -50), 120, (t_material*)&no_material);

    list_lights[0] = (t_hittable*)(&l6);
    list_lights[1] = (t_hittable*)(&l2);
    const t_hittablelist lights = hittablelist(list_lights, 2);

    debug("Start of minirt %s", "helllo !! ");
    if (!init_window(&data))
        return (EXIT_FAILURE);

    data.world = world;
    data.lights = lights;

    render(&data, &world, &lights);

    mlx_resize_hook(data.mlx, &_resize_hook, (void *)&data);

    mlx_loop_hook(data.mlx, &hook, (void *)&data);
    mlx_loop(data.mlx);
    ft_printf("\nbyebye!\n");
    mlx_terminate(data.mlx);

    return (EXIT_SUCCESS);
}



int main_checkerfloors()
{
	t_mrt data;
	if (!init_data(&data))
		return (1);

	/***************************** */
	/* 			camera 			   */
	/***************************** */
	//-422.000000 307.999863 -71.947640
	t_point3 center = point3(-422, 307, -71);
	t_vec3 direction = vec3(-1,0,0);
 	init_cam(&data.cam, center, direction, 60);
	data.cam.print((void*)(&(data.cam)));

	/***************************** */
	/* 		ambient light		   */
	/***************************** */
	ambient(&data.cam.ambient, 0.5, rgb(110,110,110));
	data.cam.ambient.print((void*)&data.cam.ambient);

	// world
	// ================================================== world ==================================================
	t_hittable *list[15];
	// ================================================== world ==================================================

	// red sphere
	// t_sphere s1 = sphere(vec3(190, 90, 190), 180, rgb(166, 13, 13));


	// checker texture sphere
	t_lambertian lambertian_material;
	t_checker_texture checker_texture1;
	t_rgb even1 = color_to_rgb(color(0.2, 0.3, 0.1));
	t_rgb odd1 = color_to_rgb(color(0.9, 0.9, 0.9));
	checker_texture_init(&checker_texture1, 100.0, even1, odd1);
	lambertian_init_tex(&lambertian_material, (t_texture*)&(checker_texture1));

	//red metallic
	t_rgb albedo = color_to_rgb(color(0.8, 0.1, 0.1));
	double fuzz = 0.0;
	t_metal metal;
	metal_init(&metal, albedo, fuzz);

	// red metallic sphere
	t_sphere s1;
	sphere_mat(&s1, point3( 90,190,90 ), 180, (t_material*)&metal);
	s1.print((void*)&s1);

	/***********************************/
	/* 			light        		   */
	/***********************************/

	// L -40.0,50.0,0.0           (0.6) strength from 0 to 1 -  rgb (10,0,255)
	// strength - 0.5
	// max 100
	// rgb (10,0,255) - color (0.5, 0, 1)  * max 200  * strength
	// color (100,100,100)

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
	t_quad s6;
	quad_mat(&s6, point3(343,554,332), vec3(-130,0,0), vec3(0,0,-105), (t_material*)&difflight);
	s6.print((void*)&s6);

	// t_sphere s6 = sphere_mat(point3( 343,554,332 ), 90, rgb(255,223 ,34 ), (t_material*)&difflight);
	// t_sphere s2 = sphere(vec3(0,250,-50), 120, rgb(16, 13, 166));
	t_sphere s2;
	sphere_mat(&s2, point3( 0,250,-50 ), 120, (t_material*)&difflight2);
	s2.print((void*)&s2);
	// t_sphere s6 = sphere_mat(point3( 343,554,332), 90, (t_material*)&difflight);

	// adding another sphere
	// red sphere
	t_sphere s4;
	sphere(&s4, vec3(400, 90, 190), 90, rgb(166, 13, 13));
	s4.print((void*)&s4);

	// add a quad just left of the s4 sphere
	t_quad s5;
	quad_rgb(&s5, point3(300, 90, 100), vec3(50,0,100), vec3(0,100,50), rgb(166, 13, 13));
	s5.print((void*)&s5);

	// add a plane just below the s4 sphere
	t_plane s7;
	plane_mat(&s7, point3(400, 0, 190), vec3(0,1,0), (t_material*)&lambertian_material);
	s7.print((void*)&s7);

	t_disk s8;
	disk(&s8, point3(500, 90, 190), vec3(0,0,1), 100, rgb(166, 53, 13));
	s8.print((void*)&s8);

// try with cube t_box box(t_point3 a, t_point3 b, t_material *mat)
	t_box s9;
	box(&s9, point3(600, 90, 190), point3(700, 190, 290), (t_material*)&metal);
	s9.print((void*)&s9);

	t_triangle s10;
	triangle(&s10, point3(300, 101, 100), point3(200, 101, 290), point3(50, 101, 190), rgb(166, 103, 13));
	s10.print((void*)&s10);

	t_sphere s11;
	sphere_mat(&s11, point3(650, 300, 200), 100, (t_material*)&lambertian_material);
	s11.print((void*)&s11);

	t_plane pl12;
	plane_mat(&pl12, point3(0, 0, -300), vec3(0,0,1), (t_material*)&lambertian_material);
	pl12.print((void*)&pl12);

	t_plane pl13;
	plane_mat(&pl13, point3(-300, 0, 0), vec3(1,0,0), (t_material*)&lambertian_material);
	pl12.print((void*)&pl12);

	/***********************************/
	/* 			earth       		   */
	/***********************************/
	t_sphere s12;
	t_lambertian earth_surface;
	img_texture_init(&s12.img_texture,"rtw_image/earthmap.jpg");
	lambertian_init_tex(&earth_surface, (t_texture*)&s12.img_texture);
	sphere_mat(&s12, point3(250, 100, -200), 100.0, (t_material*)&earth_surface);
	s12.print((void*)&s12);

	t_cylinder_capped s13;
	cylinder_capped(&s13, point3(350, 100, -300), vec3(0,1,0), 200, 50, rgb(166, 103, 13));
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
	list[12] = (t_hittable*)(&pl12);
	list[13] = (t_hittable*)(&pl13);

	const t_hittablelist world = hittablelist(list, 14);

	t_hittable *list_lights[2];

	t_empty_material empty_material;
	t_material *no_material = (t_material*)&empty_material;
	t_quad l6;
	quad_mat(&l6, point3(343,554,332), vec3(-130,0,0), vec3(0,0,-105), (t_material*)&no_material);
	l6.print((void*)&l6);

	// t_sphere s6 = sphere_mat(point3( 343,554,332 ), 90, rgb(255,223 ,34 ), (t_material*)&difflight);
	t_sphere l2;
	sphere_mat(&l2, point3( 0,250,-50 ), 120, (t_material*)&no_material);
	l2.print((void*)&l2);

	list_lights[0] = (t_hittable*)(&l6);
	list_lights[1] = (t_hittable*)(&l2);
	const t_hittablelist lights = hittablelist(list_lights, 2);

    debug("Start of minirt %s", "helllo !! ");
	if (!init_window(&data))
		return (EXIT_FAILURE);

	mlx_set_window_title(data.mlx, "New Title");
	data.world = world;
	data.lights = lights;

	render(&data, &world, &lights);

	mlx_resize_hook(data.mlx, &_resize_hook, (void *)&data);

    mlx_loop_hook(data.mlx, &hook, (void *)&data);
    mlx_loop(data.mlx);
    ft_printf("\nbyebye!\n");
    mlx_terminate(data.mlx);

    return (EXIT_SUCCESS);
}


int main_earth_nolight_pinkambient()
{
    t_mrt data;

	if (!init_data(&data))
        return (1);

	/***************************** */
	/* 			camera 			   */
	/***************************** */
	t_point3 center = point3(200, -222, -655);
	t_vec3 direction = vec3(0,100,200);
	init_cam(&data.cam, center, direction, 60);
	data.cam.print((void*)(&(data.cam)));

	/***************************** */
	/* 		ambient light		   */
	/***************************** */
	ambient(&data.cam.ambient, 1, rgb(110,110,110));
	data.cam.ambient.print((void*)&data.cam.ambient);

	// world
	t_hittable *list[7];

	// red sphere
	// t_sphere s1;
	// sphere(&s1, vec3(0, 0, -2.0), 1, rgb(128,0,0));
	// s1.print((void*)&s1);

	// // checker texture sphere
	// t_sphere s2;
	// t_rgb even1 = color_to_rgb(color(0.2, 0.3, 0.1));
	// t_rgb odd1 = color_to_rgb(color(0.9, 0.9, 0.9));
	// checker_texture_init(&s2.checker, 0.31, even1, odd1);
	// lambertian_init_tex(&s2.lambertian_mat, (t_texture*)&(s2.checker));
	// sphere_mat(&s2, point3(0, -500.5, -1), 1000, (t_material*)&s2.lambertian_mat);

	// // yellow sphere
	// t_sphere s3;
	// sphere(&s3, vec3(-1, 0.0, -2.0), 1, rgb(255,219,0));
	// s3.print((void*)&s3);

	// // white
	// t_sphere s4;
	// sphere(&s4, vec3(1, 0.0, -2.0), 1, rgb(255,255,254));
	// s4.print((void*)&s4);

	/***********************************/
	/* 			earth       		   */
	/***********************************/
	// t_sphere s5;
	// t_lambertian earth_surface;
	// t_solid_color blue;
	// // img_texture_init(&s5.img_texture,"rtw_image/earthmap.jpg");
	// solid_color_init(&blue, color(0.5,0.5,0.9));
	// lambertian_init_tex(&earth_surface, (t_texture*)&blue);
	// sphere_mat(&s5, point3(250, 100, -200), 200.0, (t_material*)&earth_surface);
	// s5.print((void*)&s5);

	t_sphere s5;
	sphere(&s5, point3(250, 100, -200), 200.0, color_to_rgb(color(0.9, 0.01, 0.1)));
	s5.print((void*)&s5);
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
	// t_quad s6;
	// quad_mat(&s6, point3(50, 20, 20), vec3(-30,0,0), vec3(0,0,-30), (t_material*)&difflight);
	
	// list[0] = (t_hittable*)(&s1);
	// list[1] = (t_hittable*)(&s2);
	// list[2] = (t_hittable*)(&s3);
	// list[3] = (t_hittable*)(&s4);
	// list[4] = (t_hittable*)(&s5);
	// list[5] = (t_hittable*)(&s6);
	list[0] = (t_hittable*)(&s5);
	// list[1] = (t_hittable*)(&s6);

	const t_hittablelist world = hittablelist(NULL, 0);

	t_hittable *list_lights[1];

	t_empty_material empty_material;
	t_material *no_material = (t_material*)&empty_material;
	t_quad l6;
	quad_mat(&l6, point3(0,0,0), vec3(0,1,0), vec3(1,0,0), (t_material*)&no_material);

	// t_sphere l6;
	// sphere_mat(&l6, point3(250, 100, -200), 200.0, (t_material*)&no_material);

	l6.print((void*)&l6);

	list_lights[0] = (t_hittable *) &l6;
	// list_lights[0] = (t_hittable*)(&l6);
	const t_hittablelist lights = hittablelist(NULL, 1);
    debug("Start of minirt %s", "helllo !! ");
	if (!init_window(&data))
		return (EXIT_FAILURE);

	data.world = world;
	data.lights = lights;
	render(&data, &world, &lights);

    mlx_loop_hook(data.mlx, &hook, (void *)&data);
	mlx_resize_hook(data.mlx, &_resize_hook, (void *)&data);
    mlx_loop(data.mlx);
    ft_printf("\nbyebye!\n");
    mlx_terminate(data.mlx);

    return (EXIT_SUCCESS);
}
