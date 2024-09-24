/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 17:31:01 by lbrusa            #+#    #+#             */
/*   Updated: 2024/09/24 11:42:28 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "debug.h"
#include "libft.h"
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

#define ROTATION_DEG 0.005
#define WINDOW_TITLE "miniRT"
#define BPP sizeof(int32_t)
#define TRUE 1
#define FALSE 0

int main_checkerfloors();
int main_earth(int argc, char **argv);
int main_blue_red();
int main_redlight(int argc, char **argv);
int main_cyl(int argc, char **argv);
int main_camera_center();
void render_from_file(char *filename);
int init_window(t_mrt *data);
bool init_data(t_mrt *data);
void exit_gracefully(mlx_t *mlx);
void rotate_camera_yaw(t_camera *cam, double angle);
void rotate_camera_pitch(t_camera *cam, double angle);
void rotate_camera_roll(t_camera *cam, double angle);
void move_camera_forward(t_camera *cam, double distance);
void move_camera_right(t_camera *cam, double distance);
void move_camera_up(t_camera *cam, double distance);
void	_resize_hook(int new_width, int new_height, void *params);
void	hook(void *param);

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
		int scene = 7;

		switch (scene)
		{
		case 1:
			main_redlight(argc, argv);
			break;
		case 2:
			main_earth(argc, argv);
			break;
		case 3:
			main_blue_red();
			break;
		case 4:
			main_checkerfloors();
			break;
		case 5:
			main_redlight(argc, argv);
			break;
		case 6:
			main_cyl(argc, argv);
			break;
		case 7:
			main_camera_center();
			break;
		default:
			break;
		}
		return (EXIT_SUCCESS);
	}
}


void key_callback(mlx_key_data_t keydata, void* param)
{
    t_mrt *data = (t_mrt *)param;
    // mlx_t *mlx = data->mlx;

	// keydata.action = MLX_PRESS;
	// bool ctrl_pressed = false;
	// if (keydata.modifier == MLX_KEY_LEFT_CONTROL || keydata.modifier == MLX_KEY_RIGHT_CONTROL)
		// ctrl_pressed = true;MLX_PRESS

	debug("keydata key action modifier %d %d %d\n", keydata.key, keydata.action, keydata.modifier);
//if (keydata.key == MLX_KEY_A && keydata.action == MLX_RELEASE && keydata.modifier == MLX_CONTROL)

	if (keydata.key == MLX_KEY_UP && keydata.action == MLX_PRESS && keydata.modifier == MLX_CONTROL)
	{
		debug("Ctrl + Arrow Up pressed\n");
		rotate_camera_pitch(&(data->cam), degrees_to_radians(data->cam.hfov * ROTATION_DEG));
		debug("camera center point = %f %f %f\n", data->cam.center.x, data->cam.center.y, data->cam.center.z);
		data->needs_render = true;
	}
	if (keydata.key == MLX_KEY_DOWN && keydata.action == MLX_PRESS && keydata.modifier == MLX_CONTROL)
	{
		debug("Ctrl + Arrow Down pressed\n");
		rotate_camera_pitch(&(data->cam), degrees_to_radians(data->cam.hfov * -ROTATION_DEG));
		debug("camera center point = %f %f %f\n", data->cam.center.x, data->cam.center.y, data->cam.center.z);
		data->needs_render = true;
	}
	if (keydata.key == MLX_KEY_LEFT && keydata.action == MLX_PRESS && keydata.modifier == MLX_CONTROL)
	{
		debug("Ctrl + Arrow Left pressed\n");
		rotate_camera_yaw(&(data->cam), degrees_to_radians(data->cam.hfov * ROTATION_DEG));
		data->needs_render = true;
		debug("camera center point = %f %f %f\n", data->cam.center.x, data->cam.center.y, data->cam.center.z);
	}
	if (keydata.key == MLX_KEY_RIGHT && keydata.action == MLX_PRESS && keydata.modifier == MLX_CONTROL)
	{
		debug("Ctrl + Arrow Right pressed\n");
		rotate_camera_yaw(&(data->cam), degrees_to_radians(data->cam.hfov * -ROTATION_DEG));
		data->needs_render = true;
		debug("camera center point = %f %f %f\n", data->cam.center.x, data->cam.center.y, data->cam.center.z);
	}
	if (keydata.key == MLX_KEY_F1 && keydata.action == MLX_PRESS && keydata.modifier == MLX_CONTROL)
	{
		debug("F1 pressed\n");
		data->cam.center = data->cam.original_pos;
		data->cam.direction = data->cam.original_dir;
		update_cam_resize(&data->cam, data->cam.image_width, data->cam.image_height);
		data->needs_render = true;
		debug("camera center point = %f %f %f\n", data->cam.center.x, data->cam.center.y, data->cam.center.z);
	}
	if (keydata.key == MLX_KEY_F && keydata.action == MLX_PRESS && keydata.modifier == MLX_CONTROL)
	{
		data->cam.hfov += 1;
		update_cam_orientation(&data->cam);
		data->needs_render = true;
		debug("F key pressed %f\n", data->cam.hfov);
		debug("camera center point = %f %f %f\n", data->cam.center.x, data->cam.center.y, data->cam.center.z);
	}
	if (keydata.key == MLX_KEY_F && keydata.action == MLX_PRESS && keydata.modifier == 0)
	{
		data->cam.hfov -= 1;
		update_cam_orientation(&data->cam);
		data->needs_render = true;
		debug("F key pressed %f\n", data->cam.hfov);
		debug("camera center point = %f %f %f\n", data->cam.center.x, data->cam.center.y, data->cam.center.z);
	}
	if (keydata.key == MLX_KEY_UP && keydata.action == MLX_PRESS && keydata.modifier == 0)
	{
		move_camera_up(&(data->cam), data->cam.image_height / 20);
		data->needs_render = true;
		debug("camera center point = %f %f %f\n", data->cam.center.x, data->cam.center.y, data->cam.center.z);
	}
	if (keydata.key == MLX_KEY_DOWN && keydata.action == MLX_PRESS && keydata.modifier == 0)
	{
		move_camera_up(&(data->cam), -data->cam.image_height / 20);
		data->needs_render = true;
		debug("camera center point = %f %f %f\n", data->cam.center.x, data->cam.center.y, data->cam.center.z);
	}
	if (keydata.key == MLX_KEY_LEFT && keydata.action == MLX_PRESS && keydata.modifier == 0)
	{
		move_camera_right(&(data->cam), -data->cam.image_width / 20);
		data->needs_render = true;
		debug("camera center point = %f %f %f\n", data->cam.center.x, data->cam.center.y, data->cam.center.z);
	}
	if (keydata.key == MLX_KEY_RIGHT && keydata.action == MLX_PRESS && keydata.modifier == 0)
	{
		move_camera_right(&(data->cam), data->cam.image_width / 20);
		data->needs_render = true;
		debug("camera center point = %f %f %f\n", data->cam.center.x, data->cam.center.y, data->cam.center.z);
	}
	if (keydata.key == MLX_KEY_SPACE && keydata.action == MLX_PRESS && keydata.modifier == 0)
	{
		move_camera_forward(&(data->cam), -data->cam.image_width / 20);
		data->needs_render = true;
		debug("camera center point = %f %f %f\n", data->cam.center.x, data->cam.center.y, data->cam.center.z);

	}
	if (keydata.key == MLX_KEY_LEFT_SHIFT && keydata.action == MLX_PRESS && keydata.modifier == 0)
	{
		move_camera_forward(&(data->cam), data->cam.image_width / 10);
		data->needs_render = true;
		debug("camera center point = %f %f %f\n", data->cam.center.x, data->cam.center.y, data->cam.center.z);
	}
    
    if (data->needs_render)
    {
        data->needs_render = false;
        render(data, &(data->world), &(data->lights));
    }
}

int	main_camera_center()
{
    t_mrt data;
	init_data(&data);

	/***************************** */
	/* 			camera 			   */	
	/***************************** */
	t_point3 center = point3(0, 0, 0);
	t_vec3 direction = vec3(0,0,1);
	init_cam(&data.cam, center, direction, 120);
	data.cam.print((void*)(&(data.cam)));

	/***************************** */
	/* 		ambient light		   */	
	/***************************** */
	ambient(&data.cam.ambient_light, 0.3, rgb(110,100,100));
	data.cam.ambient_light.print((void*)&data.cam.ambient_light);

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
	mlx_key_hook(data.mlx, key_callback, &data);
    mlx_loop_hook(data.mlx, &hook, (void *)&data);
    mlx_loop(data.mlx);
    ft_printf("\nbyebye!\n");
    mlx_terminate(data.mlx);
    return (EXIT_SUCCESS);
}

// Function to rotate a vector using a 3x3 rotation matrix
t_vec3 rotate_vector(t_vec3 v, double matrix[3][3]) {
    t_vec3 result;
    result.x = matrix[0][0] * v.x + matrix[0][1] * v.y + matrix[0][2] * v.z;
    result.y = matrix[1][0] * v.x + matrix[1][1] * v.y + matrix[1][2] * v.z;
    result.z = matrix[2][0] * v.x + matrix[2][1] * v.y + matrix[2][2] * v.z;
    return result;
}

// Function to create a yaw rotation matrix (around the y-axis)
void create_yaw_rotation_matrix(double angle, double matrix[3][3]) {
    double cos_angle = cos(angle);
    double sin_angle = sin(angle);

    matrix[0][0] = cos_angle; matrix[0][1] = 0;         matrix[0][2] = sin_angle;
    matrix[1][0] = 0;         matrix[1][1] = 1;         matrix[1][2] = 0;
    matrix[2][0] = -sin_angle; matrix[2][1] = 0;         matrix[2][2] = cos_angle;
}

// Function to create a pitch rotation matrix (around the x-axis)
void create_pitch_rotation_matrix(double angle, double matrix[3][3]) {
    double cos_angle = cos(angle);
    double sin_angle = sin(angle);

    matrix[0][0] = 1; matrix[0][1] = 0;         matrix[0][2] = 0;
    matrix[1][0] = 0; matrix[1][1] = cos_angle; matrix[1][2] = -sin_angle;
    matrix[2][0] = 0; matrix[2][1] = sin_angle; matrix[2][2] = cos_angle;
}

// Function to create a roll rotation matrix (around the z-axis)
void create_roll_rotation_matrix(double angle, double matrix[3][3]) {
    double cos_angle = cos(angle);
    double sin_angle = sin(angle);

    matrix[0][0] = cos_angle; matrix[0][1] = -sin_angle; matrix[0][2] = 0;
    matrix[1][0] = sin_angle; matrix[1][1] = cos_angle;  matrix[1][2] = 0;
    matrix[2][0] = 0;         matrix[2][1] = 0;          matrix[2][2] = 1;
}

// Function to rotate the camera around the y-axis (yaw)
void rotate_camera_yaw(t_camera *cam, double angle) {
    double rotation_matrix[3][3];
    create_yaw_rotation_matrix(angle, rotation_matrix);

    cam->u = rotate_vector(cam->u, rotation_matrix);
    cam->v = rotate_vector(cam->v, rotation_matrix);
    cam->w = rotate_vector(cam->w, rotation_matrix);

    update_cam_orientation(cam);
}

// Function to rotate the camera around the x-axis (pitch)
void rotate_camera_pitch(t_camera *cam, double angle) {
    double rotation_matrix[3][3];
    create_pitch_rotation_matrix(angle, rotation_matrix);

    cam->u = rotate_vector(cam->u, rotation_matrix);
    cam->v = rotate_vector(cam->v, rotation_matrix);
    cam->w = rotate_vector(cam->w, rotation_matrix);

    update_cam_orientation(cam);
}

// Function to rotate the camera around the z-axis (roll)
void rotate_camera_roll(t_camera *cam, double angle) {
    double rotation_matrix[3][3];
    create_roll_rotation_matrix(angle, rotation_matrix);

    cam->u = rotate_vector(cam->u, rotation_matrix);
    cam->v = rotate_vector(cam->v, rotation_matrix);
    cam->w = rotate_vector(cam->w, rotation_matrix);

    update_cam_orientation(cam);
}

// void mouse_button_callback(mouse_key_t button, action_t action, modifier_key_t mods, void* param) {
//     t_mrt* data = (t_mrt *)param;
// 	(void)mods;
//     if (button == MLX_MOUSE_BUTTON_LEFT) {
//         if (action == MLX_PRESS) {
// 			debug("Mouse pressed\n");
// 			data->mouse_state.last_x = data->mouse_state.last_y = 0;
// 			data->mouse_state.mouse_pressed = 1;
//         } else if (action == MLX_RELEASE) {
// 			data->mouse_state.mouse_pressed = 0;
// 			debug("Mouse released\n");
// 			update_cam_orientation(&(data->cam));
// 			data->needs_render = true;
//         }
//     }
// }

// void mouse_move_callback(double xpos, double ypos, void* param) {
//     t_mrt* data = (t_mrt *)param;

//     if (data->mouse_state.mouse_pressed) {
//         double deltax = xpos - data->mouse_state.last_x;
//         double deltay = ypos - data->mouse_state.last_y;

//         // Update the camera direction 
//         debug("Mouse moved: deltax = %f, deltay = %f\n", deltax, deltay);
// 		rotate_camera_yaw(&(data->cam), deltax);
// 		update_cam_orientation(&(data->cam));
//         data->mouse_state.last_x = xpos;
//         data->mouse_state.last_y = ypos;

// 		// data->needs_render = true;
//     }
// }


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

void translate_camera(t_camera *cam, t_vec3 translation) {
    cam->center = vec3add(cam->center, translation);
    update_cam_resize(cam, cam->image_width, cam->image_height);
}

void move_camera_forward(t_camera *cam, double distance) {
    t_vec3 translation = vec3multscalar(cam->w, distance);
    translate_camera(cam, translation);
}

// Function to calculate the new camera direction vector (pointing toward the origin)
t_point3 calculate_direction(t_point3 camera_pos) {
    // Direction vector from the camera to the origin (0, 0, 0)
    t_point3 direction = point3(-camera_pos.x, -camera_pos.y, -camera_pos.z);

    // Normalize the direction vector
    return unit_vector(direction);
void move_camera_right(t_camera *cam, double distance) {
    t_vec3 translation = vec3multscalar(cam->u, distance);
    translate_camera(cam, translation);
}

void move_camera_up(t_camera *cam, double distance) {
    t_vec3 translation = vec3multscalar(cam->v, distance);
    translate_camera(cam, translation);
}

void	hook(void *param)
{
	mlx_t		*mlx;
	t_mrt		*data;


	data = (t_mrt *)param;
	mlx = data->mlx;

	// if (mlx_is_key_down(mlx, MLX_KEY_ESCAPE))
	// 	exit_gracefully(mlx);
	// if (mlx_is_key_down(mlx, MLX_KEY_LEFT_CONTROL) || mlx_is_key_down(mlx, MLX_KEY_RIGHT_CONTROL))
    // {
    //     if (mlx_is_key_down(mlx, MLX_KEY_UP))
    //     {
    //         debug("Ctrl + Arrow Up pressed\n");
	// 		debug("camera center point before = %f %f %f\n", data->cam.center.x, data->cam.center.y, data->cam.center.z);
	// 		rotate_camera_pitch(&(data->cam), degrees_to_radians(data->cam.hfov * ROTATION_DEG) );
	// 		debug("camera center point = %f %f %f\n", data->cam.center.x, data->cam.center.y, data->cam.center.z);
	// 		data->needs_render = true;
	// 	}
	// 	if (mlx_is_key_down(mlx, MLX_KEY_DOWN))
    //     {
    //         debug("Ctrl + Arrow down pressed\n");
	// 					debug("camera center point before = %f %f %f\n", data->cam.center.x, data->cam.center.y, data->cam.center.z);
    //         rotate_camera_pitch(&(data->cam), degrees_to_radians(data->cam.hfov * -ROTATION_DEG));
	// 		debug("camera center point = %f %f %f\n", data->cam.center.x, data->cam.center.y, data->cam.center.z);
	// 		data->needs_render = true;
	// 	}
	// 	if (mlx_is_key_down(mlx, MLX_KEY_LEFT))
	// 	{
	// 		debug("Ctrl + Arrow left pressed\n");
	// 		rotate_camera_yaw(&(data->cam),  degrees_to_radians(data->cam.hfov * ROTATION_DEG));
	// 		debug("camera center point = %f %f %f\n", data->cam.center.x, data->cam.center.y, data->cam.center.z);
	// 		data->needs_render = true;
	// 	}
	// 	if (mlx_is_key_down(mlx, MLX_KEY_RIGHT))
	// 	{
	// 		debug("Ctrl + Arrow right pressed\n");
	// 		rotate_camera_yaw(&(data->cam), degrees_to_radians(data->cam.hfov * -ROTATION_DEG));
	// 		debug("camera center point = %f %f %f\n", data->cam.center.x, data->cam.center.y, data->cam.center.z);
	// 		data->needs_render = true;
	// 	}
	// 	if (mlx_is_key_down(mlx, MLX_KEY_F1))
	// 	{
	// 		// reset camera to original position
	// 		debug("F1 pressed\n");
	// 		data->cam.center = data->cam.original_pos;
	// 		data->cam.direction = data->cam.original_dir;
	// 		update_cam_orientation(&data->cam);
	// 		data->needs_render = true;
	// 	}
	// 	if (mlx_is_key_down(mlx, MLX_KEY_F))
	// 	{
	// 		data->cam.hfov += 1;
	// 		update_cam_orientation(&data->cam);
	// 		data->needs_render = true;
	// 		debug("F key pressed %f\n", data->cam.hfov);
	// 	}
    // }
	// if (mlx_is_key_down(mlx, MLX_KEY_F))
	// {
	// 	data->cam.hfov -= 1;
	// 	update_cam_orientation(&data->cam);
	// 	data->needs_render = true;
	// 	debug("F key pressed %f\n", data->cam.hfov);
	// }
	// if (mlx_is_key_down(mlx, MLX_KEY_UP))
	// {
	// 	move_camera_up(&(data->cam), data->cam.image_height / 20);
	// 	debug("UP key pressed");
	// 	debug("camera center point = %f %f %f\n", data->cam.center.x, data->cam.center.y, data->cam.center.z);
	// 	data->needs_render = true;
	// }
	// if (mlx_is_key_down(mlx, MLX_KEY_DOWN))
	// {
	// 	move_camera_up(&(data->cam), -data->cam.image_height / 20 );
	// 	debug("DOWN key pressed");
	// 	debug("camera center point = %f %f %f\n", data->cam.center.x, data->cam.center.y, data->cam.center.z);
	// 	data->needs_render = true;
	// }
	// if (mlx_is_key_down(mlx, MLX_KEY_LEFT))
	// {
	// 	move_camera_right(&(data->cam), -data->cam.image_width / 20);
	// 	debug("LEFT key pressed");
	// 	debug("camera center point = %f %f %f\n", data->cam.center.x, data->cam.center.y, data->cam.center.z);
	// 	data->needs_render = true;
	// }
	// if (mlx_is_key_down(mlx, MLX_KEY_RIGHT)){
	// 	move_camera_right(&(data->cam), data->cam.image_width / 20);
	// 	debug("RIGHT key pressed");
	// 	debug("camera center point = %f %f %f\n", data->cam.center.x, data->cam.center.y, data->cam.center.z);
	// 	data->needs_render = true;
	// }
	// if (mlx_is_key_down(mlx, MLX_KEY_SPACE)){
	// 	move_camera_forward(&(data->cam), -data->cam.image_width / 20);
	// 	debug("S key pressed");
	// 	data->needs_render = true;
	// }
	// if (mlx_is_key_down(mlx, MLX_KEY_LEFT_SHIFT))
	// {
	// 	move_camera_forward(&(data->cam), data->cam.image_width / 10);
	// 	debug("W key pressed");
	// 	data->needs_render = true;
	// }
	if (data->needs_render)
	{
		data->needs_render = false;
		render(data, &(data->world), &(data->lights));
	}
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
    data->image = NULL;
	data->renderscene = render;
	data->needs_render = true;
	data->mouse_state.mouse_pressed = 0;
	data->mouse_state.last_x = 0;
	data->mouse_state.last_y = 0;

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

void render_from_file(char *filename)
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
	t_point3 center = point3(278, 278, -800);
	t_vec3 direction = vec3(0,0,1);
 	init_cam(&data.cam, center, direction, 70);
	data.cam.print((void*)(&(data.cam)));

		/***************************** */
	/* 		ambient light		   */	
	/***************************** */
	ambient(&data.cam.ambient_light,0.2, rgb(110,110,110));
	data.cam.ambient_light.print((void*)&	data.cam.ambient_light);

	t_diffuse_light difflight;
	t_solid_color difflight_color;
	solid_color_init(&difflight_color, color(255, 255, 255));
	diffuse_light_init(&difflight, (t_texture*)&difflight_color);

	// blue light
	t_diffuse_light blue;
	t_solid_color diff_lightblue;
	solid_color_init(&diff_lightblue, color(0, 255, 0));
	diffuse_light_init(&blue, (t_texture*)&diff_lightblue);

		// world
	// ================================================== world ==================================================
	t_hittable *list[3];

	// "red" sphere
	// t_sphere s1;
	// sphere(&s1, vec3(190, 90, 190), 180, rgb(200, 0, 0));
	t_metal metal;
	metal_init(&metal, color(0.8, 0, 0), 0.3);
	
	// red metallic sphere
	t_sphere s1;
	sphere_mat(&s1, vec3(190, 90, 190), 180, (t_material*)&metal);
	s1.print((void*)&s1);

// as light also 
	// t_sphere s2;
	// sphere_mat(&s2, point3( 343,554,332 ), 100, (t_material*)&difflight);
	// blue
	// t_sphere s3;
	// sphere_mat(&s3, point3( 343,354,332 ), 200, (t_material*)&difflight);
	t_quad s3;
	quad_mat(&s3, point3(343,554,332), vec3(-130,0,0), vec3(0,0,-105), (t_material*)&blue);
	// t_disk s3;
	// disk_mat(&s3, point3(343,554,332), vec3(0,-1,0), 200, (t_material*)&difflight);
	
	// t_disk s3;
	// disk_mat(&s3, point3(343,554,332), vec3(0,-1,0), 200, (t_material*)&blue);
	// s3.print((void*)&s3);

	// t_disk s3;
	// disk(&s3, point3(343,554,332), vec3(0,1,0), 200, rgb(255, 255, 255));

	list[0] = (t_hittable*)(&s1);
	// list[1] = (t_hittable*)(&s2);
	list[1] = (t_hittable*)(&s3);

	const t_hittablelist world = hittablelist(list, 2);

	t_hittable *list_lights[1];
	t_empty_material empty_material;
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

	list_lights[0] = (t_hittable*)(&l2);
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



int main_redlight(int argc, char **argv)
{
    t_mrt data;
	init_data(&data);
    (void)argv;
	(void)argc;

	/***************************** */
	/* 			camera 			   */	
	/***************************** */
	t_point3 center = point3(378, 378, -1800);
	t_vec3 direction = vec3(0,0,800);
	init_cam(&data.cam, center, direction, 40);
	data.cam.print((void*)(&(data.cam)));

	/***************************** */
	/* 		ambient light		   */	
	/***************************** */
	ambient(&data.cam.ambient_light, 0.3, rgb(110,100,100));
	data.cam.ambient_light.print((void*)&data.cam.ambient_light);

	/***********************************/
	/* 			light        		   */
	/***********************************/
	t_diffuse_light difflight;
	t_solid_color difflight_color;
	solid_color_init(&difflight_color, color(100, 100, 100));
	diffuse_light_init(&difflight, (t_texture*)&difflight_color);

	t_diffuse_light blue;
	t_solid_color difflight_color2;
	solid_color_init(&difflight_color2, color(0, 0, 80));
	diffuse_light_init(&blue, (t_texture*)&difflight_color2);

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

	// t_sphere s6 = sphere_mat(point3( 343,554,332 ), 90, rgb(255,223 ,34 ), (t_material*)&difflight);
	t_sphere s2;
	sphere_mat(&s2, point3( 90,190,90 ), 60, (t_material*)&blue);
	// t_sphere s6 = sphere_mat(point3( 343,554,332), 90, (t_material*)&difflight);

	t_sphere s4;
	sphere(&s4, vec3(350, 350, 250), 160, rgb(10,0,0));

	t_sphere s5;
	sphere(&s5, vec3(0, 0, -1.2), 1, rgb(128,0,0));
	s5.print((void*)&s5);
	t_sphere s8;
	sphere(&s8, vec3(0, -100.5, -1), 200, rgb(0,128,0));
	s8.print((void*)&s8);
	t_sphere s7;
	sphere(&s7, vec3(-1, 0.0, -1.0), 1, rgb(128,128,0));
	s7.print((void*)&s7);

	list[0] = (t_hittable*)(&s1); // red sphere
	list[1] = (t_hittable*)(&s6);  // light quad
	// list[1] = (t_hittable*)(&s2);
	list[2] = (t_hittable*)(&s4);
	list[3] = (t_hittable*)(&s5);
	list[4] = (t_hittable*)(&s7);
	list[5] = (t_hittable*)(&s8);

	const t_hittablelist world = hittablelist(list, 6);

	t_empty_material empty_material;
	t_material *no_material = (t_material*)&empty_material;
	t_sphere l2;
	sphere_mat(&l2, point3( 343,554,332 ), 10, (t_material*)&no_material);

	// t_quad s6;
	// quad_mat(&s6, point3(343,554,332), vec3(-130,0,0), vec3(0,0,-105), (t_material*)&difflight);
	t_quad l6;
	quad_mat(&l6, point3(343,554,332), vec3(-200,0,0), vec3(0,0,-200), (t_material*)&no_material);

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
 	// mlx_mouse_hook(data.mlx, mouse_button_callback, (void *)&data);
    // mlx_cursor_hook(data.mlx, mouse_move_callback, (void *)&data);


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



int main_cyl(int argc, char **argv)
{
    t_mrt data;
    (void)argv;
	(void)argc;

	/***************************** */
	/* 			camera 			   */	
	/***************************** */
	t_point3 center = point3(0, 100, 400);
	t_vec3 direction = vec3(0,0,-400);
 	init_cam(&data.cam, center, direction, 60);
	data.cam.print((void*)(&(data.cam)));

	/***************************** */
	/* 		ambient light		   */	
	/***************************** */
	ambient(&data.cam.ambient_light, 1, rgb(255,255,255));


	// world
	// ================================================== world ==================================================
	t_hittable *list[2];
	// =============================================
	// t_cylinder_capped c0;
	// cylinder_capped(&c0, point3(0, 0, 0), vec3(0,1,0), 200, 50, rgb(166, 103, 13));
	// c0.print((void*)&c0);
	t_disk d0;
	disk(&d0, point3(0, 0, 0), vec3(0,1,0), 100, rgb(166, 13, 103));
	d0.print((void*)&d0);

	t_color albedo = color(0.1, 0.8, 0.1);
	double fuzz = 0.0;
	t_metal metal;
	metal_init(&metal, albedo, fuzz);
	// t_disk d1;
	// disk_mat(&d1, point3(0, 0, 0), vec3(0,1,0), 50, (t_material*)&metal);
	// d1.print((void*)&d1);
	
	// t_cylinder_capped c0;
	t_cylinder c0;
	cylinder_mat_uncapped(&c0, point3(0, 0, 0), vec3(0,1,0), 200, 10, (t_material*)&metal);
	// cylinder_mat_capped(&c0, point3(0, 0, 0), vec3(0,1,0), 200, 50, (t_material*)&metal);

	list[0] = (t_hittable*)(&d0);
	list[1] = (t_hittable*)(&c0);

	const t_hittablelist world = hittablelist(list, 2);

	t_hittable *list_lights[2];
	t_empty_material empty_material;
	t_material *no_material = (t_material*)&empty_material;
	t_quad l6;
	quad_mat(&l6, point3(343,554,332), vec3(-130,0,0), vec3(0,0,-105), (t_material*)&no_material);

	// t_sphere s6 = sphere_mat(point3( 343,554,332 ), 90, rgb(255,223 ,34 ), (t_material*)&difflight);
	t_sphere l2;
	sphere_mat(&l2, point3( 0,250,-50 ), 120, (t_material*)&no_material);

//0,250,-50 ), 120
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
