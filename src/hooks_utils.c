/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 15:20:15 by lbrusa            #+#    #+#             */
/*   Updated: 2024/09/26 17:32:23 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "hooks_utils.h"
#include "minirt.h"
#include "MLX42/MLX42.h"
#include "debug.h"
#include "utils.h"
#include "libft.h"
#include "camera.h"


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

void translate_camera(t_camera *cam, t_vec3 translation) {
    cam->center = vec3add(cam->center, translation);
    update_cam_resize(cam, cam->image_width, cam->image_height);
}

void move_camera_forward(t_camera *cam, double distance) 
{
    t_vec3 translation = vec3multscalar(cam->w, distance);
    translate_camera(cam, translation);
}

// Function to calculate the new camera direction vector (pointing toward the origin)
t_point3 calculate_direction(t_point3 camera_pos) {
    // Direction vector from the camera to the origin (0, 0, 0)
    t_point3 direction = point3(-camera_pos.x, -camera_pos.y, -camera_pos.z);

    // Normalize the direction vector
    return unit_vector(direction);
}

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
	// mlx start time
	double mlx_time;

	data = (t_mrt *)param;
	mlx = data->mlx;

	if (mlx_is_key_down(mlx, MLX_KEY_ESCAPE))
		exit_gracefully(mlx);
    if (mlx_is_key_down(mlx, MLX_KEY_UP))
	{
		debug("Arrow Up  - pitch up- pressed\n");
		rotate_camera_pitch(&(data->cam), degrees_to_radians(data->cam.hfov * ROTATION_DEG) );
		debug("camera center point = %f %f %f\n", data->cam.center.x, data->cam.center.y, data->cam.center.z);
		data->needs_render = true;
			mlx_time = mlx_get_time();
	}
	if (mlx_is_key_down(mlx, MLX_KEY_DOWN))
	{
		debug("pitch Arrow down pressed\n");
		rotate_camera_pitch(&(data->cam), degrees_to_radians(data->cam.hfov * -ROTATION_DEG));
		debug("camera center point = %f %f %f\n", data->cam.center.x, data->cam.center.y, data->cam.center.z);
		data->needs_render = true;
		mlx_time = mlx_get_time();
	}
	if (mlx_is_key_down(mlx, MLX_KEY_LEFT))
	{
		debug("yaw Arrow left pressed\n");
		rotate_camera_yaw(&(data->cam),  degrees_to_radians(data->cam.hfov * ROTATION_DEG));
		debug("camera center point = %f %f %f\n", data->cam.center.x, data->cam.center.y, data->cam.center.z);
		data->needs_render = true;
		mlx_time = mlx_get_time();
	}
	if (mlx_is_key_down(mlx, MLX_KEY_RIGHT))
	{
		debug("yaw + Arrow right pressed\n");
		rotate_camera_yaw(&(data->cam), degrees_to_radians(data->cam.hfov * -ROTATION_DEG));
		debug("camera center point = %f %f %f\n", data->cam.center.x, data->cam.center.y, data->cam.center.z);
		data->needs_render = true;
		mlx_time = mlx_get_time();
	}
	if (mlx_is_key_down(mlx, MLX_KEY_F1))
	{
		// reset camera to original position
		debug("F1 reset pressed\n");
		data->cam.center = data->cam.original_pos;
		data->cam.direction = data->cam.original_dir;
		update_cam_orientation(&data->cam);
		data->needs_render = true;
		mlx_time = mlx_get_time();
	}
	if (mlx_is_key_down(mlx, MLX_KEY_F))
	{
		data->cam.hfov += 1;
		update_cam_orientation(&data->cam);
		data->needs_render = true;
		debug("F key pressed %f\n", data->cam.hfov);
		mlx_time = mlx_get_time();
	}
    
	if (mlx_is_key_down(mlx, MLX_KEY_G))
	{
		data->cam.hfov -= 1;
		update_cam_orientation(&data->cam);
		data->needs_render = true;
		debug("G neg hfov key pressed %f\n", data->cam.hfov);
		mlx_time = mlx_get_time();
	}
	if (mlx_is_key_down(mlx, MLX_KEY_W))
	{
		move_camera_up(&(data->cam), data->cam.image_height / 20);
		debug("W UP key pressed");
		debug("camera center point = %f %f %f\n", data->cam.center.x, data->cam.center.y, data->cam.center.z);
		data->needs_render = true;
		mlx_time = mlx_get_time();
	}
	if (mlx_is_key_down(mlx, MLX_KEY_S))
	{
		move_camera_up(&(data->cam), -data->cam.image_height / 20 );
		debug("S DOWN key pressed");
		debug("camera center point = %f %f %f\n", data->cam.center.x, data->cam.center.y, data->cam.center.z);
		data->needs_render = true;
		mlx_time = mlx_get_time();
	}
	if (mlx_is_key_down(mlx, MLX_KEY_A))
	{
		move_camera_right(&(data->cam), -data->cam.image_width / 20);
		debug("A move left key pressed");
		debug("camera center point = %f %f %f\n", data->cam.center.x, data->cam.center.y, data->cam.center.z);
		data->needs_render = true;
		mlx_time = mlx_get_time();
	}
	if (mlx_is_key_down(mlx, MLX_KEY_D)){
		move_camera_right(&(data->cam), data->cam.image_width / 20);
		debug("D more right key pressed");
		debug("camera center point = %f %f %f\n", data->cam.center.x, data->cam.center.y, data->cam.center.z);
		data->needs_render = true;
		mlx_time = mlx_get_time();
	}
	if (mlx_is_key_down(mlx, MLX_KEY_SPACE)){
		move_camera_forward(&(data->cam), -data->cam.image_width / 20);
		debug("S key pressed");
		data->needs_render = true;
		mlx_time = mlx_get_time();
	}
	if (mlx_is_key_down(mlx, MLX_KEY_LEFT_SHIFT))
	{
		move_camera_forward(&(data->cam), data->cam.image_width / 10);
		debug("Left shift key pressed");
		data->needs_render = true;
		mlx_time = mlx_get_time();
	}
	if (data->needs_render)
	{
		if (mlx_get_time() - mlx_time > 0.005)
		{
			data->needs_render = false;
			render(data, &(data->world), &(data->lights));
		}
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