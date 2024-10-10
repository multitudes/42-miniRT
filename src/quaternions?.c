/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quaternions?.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 21:43:38 by lbrusa            #+#    #+#             */
/*   Updated: 2024/10/07 17:22:08 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// this code was used as a test to otate the camera...
// not in use right now but it's a good example of how to use quaternions
// and how to rotate a vector using them

// typedef struct s_quat
// {
// 	double			scalar;
// 	union
// 	{
// 		t_vec3		vector;
// 		struct
// 		{
// 			double	i;
// 			double	j;
// 			double	k;
// 		};
// 		struct
// 		{
// 			double	x;
// 			double	y;
// 			double	z;
// 		};
// 	};
// }					t_quat;

// // Function to normalize a quaternion
// void	normalize_quat(t_quat *q)
// {
// 	double	magnitude;

// 	magnitude = sqrt(q->scalar * q->scalar + q->x * q->x + q->y * q->y + q->z
// 			* q->z);
// 	q->scalar /= magnitude;
// 	q->x /= magnitude;
// 	q->y /= magnitude;
// 	q->z /= magnitude;
// }

// // Function to rotate a vector using a quaternion
// t_vec3	rotate_vector(t_vec3 v, t_quat q)
// {
// 	t_quat	q_conjugate = {.scalar = q.scalar, .vector = {.x = -q.x, .y = -q.y,
// 			.z;
// 	t_quat	v_quat = {.scalar = 0, .vector = {.x = v.x, .y = v.y, .z;
// 	t_quat	temp = {.scalar = q.scalar * v_quat.scalar - q.x * v_quat.x - q.y
// 			* v_quat.y - q.z * v_quat.z, .vector = {.x = q.scalar * v_quat.x
// 			+ q.x * v_quat.scalar + q.y * v_quat.z - q.z * v_quat.y,
// 			.y = q.scalar * v_quat.y - q.x * v_quat.z + q.y * v_quat.scalar
// 			+ q.z * v_quat.x, .z;
// 	t_quat	result = {.scalar = temp.scalar * q_conjugate.scalar - temp.x
// 			* q_conjugate.x - temp.y * q_conjugate.y - temp.z * q_conjugate.z,
// 			.vector = {.x = temp.scalar * q_conjugate.x + temp.x
// 			* q_conjugate.scalar + temp.y * q_conjugate.z - temp.z
// 			* q_conjugate.y, .y = temp.scalar * q_conjugate.y - temp.x
// 			* q_conjugate.z + temp.y * q_conjugate.scalar + temp.z
// 			* q_conjugate.x, .z;

// 	q_conjugate = {.scalar = q.scalar, .vector = {.x = -q.x, .y = -q.y, .z =
// 		-q.z}};
// 	v_quat = {.scalar = 0, .vector = {.x = v.x, .y = v.y, .z = v.z}};
// 	// Quaternion multiplication: q * v_quat
// 	temp = {.scalar = q.scalar  v_quat.scalar - q.x  v_quat.x - q.y  v_quat.y
// 		- q.z  v_quat.z, .vector = {.x = q.scalar  v_quat.x + q.x  v_quat.scalar
// 		+ q.y  v_quat.z - q.z  v_quat.y, .y = q.scalar  v_quat.y - q.x  v_quat.z
// 		+ q.y  v_quat.scalar + q.z  v_quat.x, .z = q.scalar * v_quat.z + q.x
// 		* v_quat.y - q.y * v_quat.x + q.z * v_quat.scalar}};
// 	// Quaternion multiplication: temp * q_conjugate
// 	result = {.scalar = temp.scalar  q_conjugate.scalar - temp.x  q_conjugate.x
// 		- temp.y  q_conjugate.y - temp.z  q_conjugate.z,
// 		.vector = {.x = temp.scalar  q_conjugate.x + temp.x  q_conjugate.scalar
// 		+ temp.y  q_conjugate.z - temp.z  q_conjugate.y,
// 		.y = temp.scalar  q_conjugate.y - temp.x  q_conjugate.z
// 		+ temp.y  q_conjugate.scalar + temp.z  q_conjugate.x, .z = temp.scalar
// 		* q_conjugate.z + temp.x * q_conjugate.y - temp.y * q_conjugate.x
// 		+ temp.z * q_conjugate.scalar}};
// 	return (vec3(result.x, result.y, result.z));
// }

// // Function to rotate the camera around the y-axis using quaternions
// void	rotate_camera_yaw(t_camera *cam, double deltax)
// {
// 	double sensitivity = 0.0022;
// 	double angle = deltax * sensitivity;
// 	printf("angle = %f\n", angle);

// 	// Create the quaternion for the yaw rotation
// 	t_quat rotation = {.scalar = cos(angle / 2), .vector = {.x = 0.0,
// 		.y = sin(angle / 2), .z = 0.0}};

// 	// Normalize the quaternion
// 	normalize_quat(&rotation);

// 	// Apply the quaternion rotation to the camera's direction vector
// 	cam->direction = rotate_vector(cam->direction, rotation);

// 	// Update the camera orientation
// 	update_cam_orientation(cam);
// }