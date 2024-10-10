/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_obj6.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 19:24:03 by lbrusa            #+#    #+#             */
/*   Updated: 2024/10/10 19:29:31 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

/* Quad light.
 * usage:
 * "l" "qd" [origin] [side_vec1] [side_vec2] [rgbcolor] [intensity ([0.0;1.0])]
*/
void	quad_light_empty(t_objects *obj, int set_index)
{
	t_init_params	params;

	if (set_index >= OBJECT_COUNT)
		call_error("exceeds_array size", "q_light", obj);
	if (count_tokens(obj->_tokens) != 7)
		call_error("invalid token amount", "q_light", obj);
	params.center = set_vec3(obj, 2, "q_light", 0);
	params.side1 = set_vec3(obj, 3, "q_light", 0);
	params.side2 = set_vec3(obj, 4, "q_light", 0);
	empty_material_init(&obj->empty_lights[set_index].no_material);
	params.mat = (t_material *) &obj->empty_lights[set_index].no_material;
	quad_mat(&obj->empty_lights[set_index].q_body, params);
}

void	sphere_light_empty(t_objects *obj, int set_index)
{
	t_init_params	params;
	char			**tokens;

	params.diam = 100;
	tokens = obj->_tokens;
	if (set_index >= OBJECT_COUNT)
		call_error("exceeds array size", "light", obj);
	if (count_tokens(tokens) != 4 && count_tokens(tokens) != 5)
		call_error("invalid token amount", "light", obj);
	if (count_tokens(tokens) == 5)
		params.diam = ft_atod(tokens[4]);
	if (params.diam < 0)
		call_error("diameter cannot be negative...", "light", obj);
	params.center = set_vec3(obj, 1, "light", 0);
	empty_material_init(&obj->empty_lights[set_index].no_material);
	params.mat = (t_material *) &obj->empty_lights[set_index].no_material;
	sphere_mat(&obj->empty_lights[set_index].s_body, params);
}
