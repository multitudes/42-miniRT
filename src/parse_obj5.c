/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_obj5.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 17:36:46 by ralgaran          #+#    #+#             */
/*   Updated: 2024/10/07 12:31:39 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

static void	cone_7_tokens(t_objects *obj, int set_index, char **tokens, \
	t_init_params params)
{
	metal_init(&obj->cones[set_index].body.metal, set_rgb(obj, 5, "cone"), \
		ft_atod(tokens[6]));
	params.mat = (t_material *)&obj->cones[set_index].body.metal;
	cone_mat(&obj->cones[set_index], params);
}

/*
 * usage:
 * default - co [apex] [axis] [diam] [height] [color]
 * metalic - co [apex] [axis] [diam] [height] [color] [fuzz]
 *  
 * apex - the pointy end of the cone
 * axis - axis of cone (goes up from the apex)
 * diam - the diameter of the base of the cone
*/
void	get_cone(t_objects *obj)
{
	static int		set_index;
	char			**tokens;
	t_init_params	params;

	tokens = obj->_tokens;
	if (set_index >= OBJECT_COUNT)
		call_error("exceeds array size", "cone", obj);
	if (count_tokens(tokens) != 6 && count_tokens(tokens) != 7)
		call_error("invalid token amount", "cone", obj);
	params.center = set_vec3(obj, 1, "cone", 0);
	params.normal = set_vec3(obj, 2, "cone", 1);
	params.diam = ft_atod(tokens[3]);
	params.height = ft_atod(tokens[4]);
	if (count_tokens(tokens) == 7)
		cone_7_tokens(obj, set_index, tokens, params);
	else
	{
		params.rgbcolor = set_rgb(obj, 5, "cone");
		cone_rgb(&obj->cones[set_index], params);
	}
	obj->hit_list[obj->hit_idx] = (t_hittable *)&obj->cones[set_index];
	obj->hit_idx++;
	set_index++;
}

static void	cone_u_7_tokens(t_objects *obj, int set_index, char **tokens, \
	t_init_params params)
{
	metal_init(&obj->cones_u[set_index].metal, set_rgb(obj, 5, "cone_u"), \
		ft_atod(tokens[6]));
	params.mat = (t_material *)&obj->cones_u[set_index].metal;
	cone_uncap_mat(&obj->cones_u[set_index], params);
}

/*
 * The UNCAPPED cone
 * usage:
 * default - co_u [apex] [axis] [diam] [height] [color]
 * metalic - co_u [apex] [axis] [diam] [height] [color] [fuzz]
 *  
 * apex - the pointy end of the cone
 * axis - axis of cone (goes up from the apex)
 * diam - the diameter of the base of the cone
*/
void	get_cone_u(t_objects *obj)
{
	static int		set_index;
	char			**tokens;
	t_init_params	params;

	tokens = obj->_tokens;
	if (set_index >= OBJECT_COUNT)
		call_error("exceeds array size", "co_u", obj);
	if (count_tokens(tokens) != 6 && count_tokens(tokens) != 7)
		call_error("invalid token amount", "co_u", obj);
	params.center = set_vec3(obj, 1, "cone_u", 0);
	params.normal = set_vec3(obj, 2, "cone_u", 1);
	params.diam = ft_atod(tokens[3]);
	params.height = ft_atod(tokens[4]);
	if (count_tokens(tokens) == 7)
		cone_u_7_tokens(obj, set_index, tokens, params);
	else
	{
		params.rgbcolor = set_rgb(obj, 5, "cone_u");
		cone_uncap_rgb(&obj->cones_u[set_index], params);
	}
	obj->hit_list[obj->hit_idx] = (t_hittable *)&obj->cones_u[set_index];
	obj->hit_idx++;
	set_index++;
}
