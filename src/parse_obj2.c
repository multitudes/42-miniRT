/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_obj2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ralgaran <ralgaran@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 17:32:22 by ralgaran          #+#    #+#             */
/*   Updated: 2024/10/03 17:32:32 by ralgaran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

static void	sphere_5_tokens(t_objects *obj, int set_index, char **tokens, \
	double diam)
{
	t_init_params	params;

	params.center = set_vec3(obj, 1, "sphere", 0);
	params.diam = diam;
	if (is_float(tokens[4]))
	{
		metal_init(&obj->spheres[set_index].metal, set_rgb(obj, 3, "sphere"),
			ft_atod(tokens[4]));
		params.mat = (t_material *)&obj->spheres[set_index].metal;
		sphere_mat(&obj->spheres[set_index], params);
	}
	else
	{
		checker_texture_init(&obj->spheres[set_index].checker, CHECKER_SIZE,
			set_rgb(obj, 3, "sphere"), set_rgb(obj, 4, "sphere"));
		lambertian_init_tex(&obj->spheres[set_index].lambertian_mat,
			(t_texture *)&obj->spheres[set_index].checker);
		params.mat = (t_material *)&obj->spheres[set_index].lambertian_mat;
		sphere_mat(&obj->spheres[set_index], params);
	}
}

static void	sphere_4_tokens(t_objects *obj, int set_index, char **tokens, \
	int diam)
{
	t_init_params	params;

	params.center = set_vec3(obj, 1, "sphere", 0);
	params.diam = diam;
	if (ft_strncmp(tokens[3], "img:", 4) == 0)
	{
		img_texture_init(&obj->spheres[set_index].img_texture, &tokens[3][4]);
		lambertian_init_tex(&obj->spheres[set_index].lambertian_mat,
			(t_texture *)&obj->spheres[set_index].img_texture);
		params.mat = (t_material *)&obj->spheres[set_index].lambertian_mat;
		sphere_mat(&obj->spheres[set_index], params);
	}
	else
	{
		params.rgbcolor = set_rgb(obj, 3, "sphere");
		sphere(&obj->spheres[set_index], params);
	}
}

/*
 * usage:
 * default sphere -		"sp" [origin] [diameter] [rgb color]
 * checker texture -	"sp" [origin] [diameter] [rgb color1] [rgb color2]
 * image (earthmap) -	"sp" [origin] [diameter] "img:"[path to .jpg]
 * metal sphere -		"sp" [origin] [diameter] [rgb color] [fuzz value(double)]
*/
void	get_sphere(t_objects *obj)
{
	static int	set_index;
	char		**tokens;
	double		diam;

	tokens = obj->_tokens;
	if (set_index >= SPHERES_COUNT)
		call_error("exceeds array size", "sphere", obj);
	if (count_tokens(tokens) != 4 && count_tokens(tokens) != 5)
		call_error("needs 4 or 5 tokens", "sphere", obj);
	diam = ft_atod(tokens[2]);
	if (diam < 0)
		call_error("diameter cannot be negative...", "sphere", obj);
	if (count_tokens(tokens) == 5)
		sphere_5_tokens(obj, set_index, tokens, diam);
	else
		sphere_4_tokens(obj, set_index, tokens, diam);
	obj->hit_list[obj->hit_idx] = (t_hittable *)&obj->spheres[set_index];
	obj->hit_idx++;
	set_index++;
}

static void	plane_5_tokens(t_objects *obj, int set_index, char **tokens, \
	t_init_params params)
{
	if (is_float(tokens[4]))
	{
		metal_init(&obj->planes[set_index].metal, set_rgb(obj, 3, "plane"), \
			ft_atod(tokens[4]));
		params.mat = (t_material *)&obj->planes[set_index].metal;
		plane_mat(&obj->planes[set_index], params);
	}
	else 
	{
		checker_texture_init(&obj->planes[set_index].checker, CHECKER_SIZE,
			set_rgb(obj, 3, "plane"), set_rgb(obj, 4, "plane"));
		lambertian_init_tex(&obj->planes[set_index].lambertian_mat,
			(t_texture *)&obj->planes[set_index].checker);
		params.mat = (t_material *)&obj->planes[set_index].lambertian_mat;
		plane_mat(&obj->planes[set_index], params);
	}
}

/*
 * usage:
 * regular plane -	"pl" [origin] [surface normal] [rgb color]
 * metal plane -	"pl" [origin] [surface normal] [rgb color] [fuzz(double)]
 * checker plane -	"pl" [origin] [surface normal] [rgb color1] [rgb color2]
 *
 * surface normal in the form - ([0;1],[0;1],[0;1])
 */
void	get_plane(t_objects *obj)
{
	static int		set_index;
	char			**tokens;
	t_init_params	params;

	tokens = obj->_tokens;
	if (set_index >= OBJECT_COUNT)
		call_error("exceeds array size", "plane", obj);
	if (count_tokens(tokens) != 4 && count_tokens(tokens) != 5)
		call_error("invalid token amount", "plane", obj);
	params.center = set_vec3(obj, 1, "plane", 0);
	params.normal = set_vec3(obj, 2, "plane", 1);
	if (count_tokens(tokens) == 5)
		plane_5_tokens(obj, set_index, tokens, params);
	else
	{
		params.rgbcolor = set_rgb(obj, 3, "plane");
		plane(&obj->planes[set_index], params);
	}
	obj->hit_list[obj->hit_idx] = (t_hittable *)&obj->planes[set_index];
	obj->hit_idx++;
	set_index++;
}
