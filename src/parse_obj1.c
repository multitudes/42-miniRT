/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_obj1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ralgaran <ralgaran@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 16:46:16 by ralgaran          #+#    #+#             */
/*   Updated: 2024/10/03 16:46:16 by ralgaran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

/* Inits the ambient struct inside of t_mrt->t_camera.
 * usage:
 * "A" [intensity([0.0;1.0])] [rgb color]
*/
void	get_ambient(t_mrt *data)
{
	static int	already_set;
	char		**tokens;

	tokens = data->objects._tokens;
	if (already_set)
		call_error("this element can only be set once", "ambient",
			&data->objects);
	if (count_tokens(tokens) != 3)
		call_error("invalid token amount", "ambient", &data->objects);
	ambient(&data->cam.ambient, ft_atod(tokens[1]), \
		set_rgb(&data->objects, 2, "ambient"));
	already_set = 1;
}

/* Inits the camera struct inside of t_mrt.
 * usage:
 * "C" [origin] [orientation(view) vector (normalized values)] [horiz. fow (double)]
*/
void	get_camera(t_mrt *data)
{
	static int	already_set;
	double		hfov;
	char		**tokens;

	tokens = data->objects._tokens;
	if (already_set)
		call_error("this element can only be set once", "camera",
			&data->objects);
	if (count_tokens(tokens) != 4)
		call_error("invalid token amount", "camera", &data->objects);
	hfov = ft_atod(tokens[3]);
	if (hfov < 0. || hfov > 180.)
		call_error("fov must be in range [0;180]", "camera", &data->objects);
	init_cam(&data->cam, set_vec3(&data->objects, 1, "camera", 0), \
		set_vec3(&data->objects, 2, "camera", 1), hfov);
	already_set = 1;
}

/* Quad light.
 * usage:
 * "l" "qd" [origin] [side_vector1] [side_vector2] [rgb color] [intensity ([0.0;1.0])]
*/
void	quad_light(t_objects *obj, int set_index)
{
	t_color			color;
	t_rgb			rgbcolor;
	t_init_params	params;

	if (set_index >= OBJECT_COUNT)
		call_error("exceeds_array size", "q_light", obj);
	if (count_tokens(obj->_tokens) != 7)
		call_error("invalid token amount", "q_light", obj);
	rgbcolor = set_rgb(obj, 5, "q_light");
	color = vec3multscalar(rgb_to_color(rgbcolor), 100
			* ft_atod(obj->_tokens[6]));
	solid_color_init(&obj->lights[set_index].color, color);
	diffuse_light_init(&obj->lights[set_index].difflight,
		(t_texture *)&obj->lights[set_index].color);
	params.center = set_vec3(obj, 2, "q_light", 0);
	params.side1 = set_vec3(obj, 3, "q_light", 0);
	params.side2 = set_vec3(obj, 4, "q_light", 0);
	params.mat = (t_material*)&obj->lights[set_index].difflight;
	quad_mat(&obj->lights[set_index].q_body, params);
}

/* Makes a light object, which is a sphere or a quad with a light texture.
 * Adds the sphere from the light struct to the regular hittable list.
 * as well as the light one.
 */
/*
 * usage (for default sphere light):
 * "l" [origin] [intensity([0.0;1,0])] [rgb color] [optional : diameter(default value=100)]
 */
void	get_light(t_objects *obj)
{
	static int	set_index;
	int			diam;
	t_color		color;
	t_rgb		rgbcolor;
	char		**tokens;

	if (ft_strncmp(obj->_tokens[1], "qd", 3) == 0)
		quad_light(obj, set_index);
	else
	{
		diam = 100;
		tokens = obj->_tokens;
		if (set_index >= OBJECT_COUNT)
			call_error("exceeds array size", "light", obj);
		if (count_tokens(tokens) != 4 && count_tokens(tokens) != 5)
			call_error("invalid token amount", "light", obj);
		rgbcolor = set_rgb(obj, 3, "light");
		color = vec3multscalar(rgb_to_color(rgbcolor), 100
				* ft_atod(tokens[2]));
		solid_color_init(&obj->lights[set_index].color, color);
		diffuse_light_init(&obj->lights[set_index].difflight,
			(t_texture *)&obj->lights[set_index].color);
		if (count_tokens(tokens) == 5)
			diam = ft_atod(tokens[4]);
		if (diam < 0)
			call_error("diameter cannot be negative...", "light", obj);
		sphere_mat(&obj->lights[set_index].s_body, set_vec3(obj, 1, "light", 0),
			diam, (t_material *)&obj->lights[set_index].difflight);
	}
	obj->hit_list[obj->hit_idx] = (t_hittable *)&obj->lights[set_index].s_body;
	obj->hit_idx++;
	obj->light_hit[obj->light_hit_idx] = (t_hittable *)&obj->lights[set_index].s_body;
	obj->light_hit_idx++;
	set_index++;
}
