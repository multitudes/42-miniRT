/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_obj4.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 16:57:36 by ralgaran          #+#    #+#             */
/*   Updated: 2024/10/10 20:31:54 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

static void	disk_6_tokens(t_objects *obj, int set_index, char **tokens, \
	t_init_params params)
{
	metal_init(&obj->disks[set_index].metal, set_rgb(obj, 4, "disk"),
		ft_atod(tokens[4]));
	params.mat = (t_material *)&obj->disks[set_index].metal;
	disk_mat(&obj->disks[set_index], params);
}

/*
 * usage:
 * default:
 *	"dsk" [origin] [surface normal] [diameter] [rgb color]
 * metalic:
 *	"dsk" [origin] [surface normal] [diameter] [rgb color] [fuzz(double)]
 */
void	get_disk(t_objects *obj)
{
	static int		set_index;
	char			**tokens;
	t_init_params	params;

	tokens = obj->_tokens;
	if (set_index >= OBJECT_COUNT)
		call_error("exceeds array size", "disk", obj);
	if (count_tokens(tokens) != 5 && count_tokens(tokens) != 6)
		call_error("invalid token amount", "disk", obj);
	params.center = set_vec3(obj, 1, "disk", 0);
	params.normal = set_vec3(obj, 2, "disk", 1);
	params.diam = ft_atod(tokens[3]);
	if (count_tokens(tokens) == 6)
		disk_6_tokens(obj, set_index, tokens, params);
	else
	{
		params.rgbcolor = set_rgb(obj, 4, "disk");
		disk(&obj->disks[set_index], params);
	}
	obj->hit_list[obj->hit_idx] = (t_hittable *)&obj->disks[set_index];
	obj->hit_idx++;
	set_index++;
}

static void	triangle_6_tokens(t_objects *obj, int set_index, char **tokens, \
	t_init_params params)
{
	metal_init(&obj->triangles[set_index].metal, set_rgb(obj, 4,
			"triangle"), ft_atod(tokens[5]));
	params.mat = (t_material *)&obj->triangles[set_index].metal;
	triangle_mat(&obj->triangles[set_index], params);
}

/*
 * usage:
 * default -	"tr" [vertice1] [vertice2] [vertice3] [rbg color]
 * metalic -	"tr" [vertice1] [vertice2] [vertice3] [rbg color] [fuzz(double)]
 * (vertices are points in 3d space)
 */
void	get_triangle(t_objects *obj)
{
	static int		set_index;
	char			**tokens;
	t_init_params	params;

	tokens = obj->_tokens;
	if (set_index >= TRIANGLE_COUNT)
		call_error("exceeds array size", "triangle", obj);
	if (count_tokens(tokens) != 5 && count_tokens(tokens) != 6)
		call_error("invalid token amount", "triangle", obj);
	params.a = set_vec3(obj, 1, "triangle", 0);
	params.b = set_vec3(obj, 2, "triangle", 0);
	params.c = set_vec3(obj, 3, "triangle", 0);
	if (count_tokens(tokens) == 6)
		triangle_6_tokens(obj, set_index, tokens, params);
	else
	{
		params.rgbcolor = set_rgb(obj, 4, "triangle");
		triangle(&obj->triangles[set_index], params);
	}
	obj->hit_list[obj->hit_idx] = (t_hittable *)&obj->triangles[set_index];
	obj->hit_idx++;
	set_index++;
}

/*
 * usage:
 * default - "box" [origin] [diagonal point] [color]
 * metalic - "box" [origin] [diagonal point] [color] [fuzz(double)]
 */
void	get_box(t_objects *obj)
{
	static int		set_index;
	t_init_params	params;

	if (set_index >= OBJECT_COUNT)
		call_error("exceeds array size", "box", obj);
	if (count_tokens(obj->_tokens) != 4 && count_tokens(obj->_tokens) != 5)
		call_error("invalid token amount", "box", obj);
	params.a = set_vec3(obj, 1, "box", 0);
	params.b = set_vec3(obj, 2, "box", 0);
	if (count_tokens(obj->_tokens) == 5)
	{
		metal_init(&obj->boxes[set_index].metal, set_rgb(obj, 3, "box"),
			ft_atod(obj->_tokens[4]));
		params.mat = (t_material *)&obj->boxes[set_index].metal;
		box(&obj->boxes[set_index], params);
	}
	else
	{
		params.rgbcolor = set_rgb(obj, 3, "box");
		box_rgb(&obj->boxes[set_index], params);
	}
	obj->hit_list[obj->hit_idx] = (t_hittable *)&obj->boxes[set_index];
	obj->hit_idx++;
	set_index++;
}
