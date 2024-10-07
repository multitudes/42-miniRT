/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_obj3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ralgaran <ralgaran@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 16:46:06 by ralgaran          #+#    #+#             */
/*   Updated: 2024/10/04 16:46:14 by ralgaran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

static void	cylinder_7_tokens(t_objects *obj, int set_index, char **tokens, \
	t_init_params params)
{
	metal_init(&obj->cylinders[set_index].cylinder_uncapped.metal,
		set_rgb(obj, 3, "cylinder"), ft_atod(tokens[4]));
	params.mat = \
		(t_material *)&obj->cylinders[set_index].cylinder_uncapped.metal;
	cylinder_mat_capped(&obj->cylinders[set_index], params);
}

/*
 * usage:
 * default, capped cylinder:
 *	"cy" [origin] [axis normal] [diameter] [height] [rgb color]
 * metalic, capped cylinder:
 *	"cy" [origin] [axis normal] [diameter] [height] [rgb color] [fuzz(double)]
 */
void	get_cylinder(t_objects *obj)
{
	static int		set_index;
	char			**tokens;
	t_init_params	params;

	tokens = obj->_tokens;
	if (set_index >= CYLINDER_COUNT)
		call_error("exceeds array size", "cylinder", obj);
	if (count_tokens(tokens) != 6 && count_tokens(tokens) != 7)
		call_error("invalid token amount", "cylinder", obj);
	params.center = set_vec3(obj, 1, "cylinder", 0);
	params.normal = set_vec3(obj, 2, "cylinder", 1);
	params.diam = ft_atod(tokens[3]);
	params.height = ft_atod(tokens[4]);
	if (count_tokens(tokens) == 7)
		cylinder_7_tokens(obj, set_index, tokens, params);
	else
	{
		params.rgbcolor = set_rgb(obj, 5, "cylinder");
		cylinder_capped(&obj->cylinders[set_index], params);
	}
	obj->hit_list[obj->hit_idx] = (t_hittable *)&obj->cylinders[set_index];
	obj->hit_idx++;
	set_index++;
}

static void	cylinder_u_7_tokens(t_objects *obj, int set_index, char **tokens, \
	t_init_params params)
{
	metal_init(&obj->cylinders_u[set_index].metal,
		set_rgb(obj, 3, "cylinder_u"), ft_atod(tokens[4]));
	params.mat = (t_material *)&obj->cylinders_u[set_index].metal;
	cylinder_mat_uncapped(&obj->cylinders_u[set_index], params);
}

/*
 * usage:
 * default, uncapped cylinder:
 * 	"cyu" [origin] [axis normal] [diameter] [height] [rgb color]
 * metalic, uncapped cylinder:
 *	"cyu" [origin] [axis normal] [diameter] [height] [rgb color] [fuzz(double)]
 */
void	get_cylinder_u(t_objects *obj)
{
	static int		set_index;
	char			**tokens;
	t_init_params	params;

	tokens = obj->_tokens;
	if (set_index >= CYLINDER_COUNT)
		call_error("exceeds array size", "cylinder_u", obj);
	if (count_tokens(tokens) != 6 && count_tokens(tokens) != 7)
		call_error("invalid token amount", "cylinder_u", obj);
	params.center = set_vec3(obj, 1, "cylinder_u", 0);
	params.normal = set_vec3(obj, 2, "cylinder_u", 1);
	params.diam = ft_atod(tokens[3]);
	params.height = ft_atod(tokens[4]);
	if (count_tokens(tokens) == 7)
		cylinder_u_7_tokens(obj, set_index, tokens, params);
	else
	{
		params.rgbcolor = set_rgb(obj, 5, "cylinder_u");
		cylinder_uncapped(&obj->cylinders_u[set_index], params);
	}
	obj->hit_list[obj->hit_idx] = (t_hittable *)&obj->cylinders_u[set_index];
	obj->hit_idx++;
	set_index++;
}

/*
 * usage:
 * default quad:
 * 	"qd" [origin] [side_vector1] [side_vector2] [color]
 * metalic quad:
 * 	"qd" [origin] [side_vector1] [side_vector2] [color] [fuzz(double)]
 * (for quad light check quad_light())
 */
void	get_quad(t_objects *obj)
{
	static int		set_index;
	char			**tokens;
	t_init_params	params;

	tokens = obj->_tokens;
	if (set_index >= OBJECT_COUNT)
		call_error("exceeds array size", "quad", obj);
	if (count_tokens(tokens) != 5 && count_tokens(tokens) != 6)
		call_error("invalid token amount", "quad", obj);
	params.center = set_vec3(obj, 1, "quad", 0);
	params.side1 = set_vec3(obj, 2, "quad", 0);
	params.side2 = set_vec3(obj, 3, "quad", 0);
	params.rgbcolor = set_rgb(obj, 4, "quad");
	if (count_tokens(tokens) == 6)
	{
		metal_init(&obj->quads[set_index].metal, set_rgb(obj, 4, "quad"),
			ft_atod(tokens[4]));
		params.mat = (t_material *)&obj->quads[set_index].metal;
		quad_mat(&obj->quads[set_index], params);
	}
	else
		quad_rgb(&obj->quads[set_index], params);
	obj->hit_list[obj->hit_idx] = (t_hittable *)&obj->quads[set_index];
	obj->hit_idx++;
	set_index++;
}
