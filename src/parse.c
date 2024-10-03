/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 21:22:36 by ralgaran          #+#    #+#             */
/*   Updated: 2024/09/30 10:00:16 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

/* Returns a t_rgb struct that contains values from the token */
static t_rgb	set_rgb(t_objects *obj, int index, char *func_name)
{
	char	**rgb_tok;
	int		rgb_val[3];
	int		i;

	rgb_tok = ft_split(obj->_tokens[index], ',');
	if (*rgb_tok == NULL)
		call_error("ft_split() fail", func_name, obj);
	i = -1;
	while (++i < 3)
	{
		rgb_val[i] = ft_atoi(rgb_tok[i]);
		if (rgb_val[i] < 0 || rgb_val[i] > 255)
		{
			free_split(rgb_tok);
			call_error("color not in rgb range", func_name, obj);
		}
	}
	free_split(rgb_tok);
	return (rgb(rgb_val[0], rgb_val[1], rgb_val[2]));
}

/*
 * Returns t_vec3 with values from the token.
 *
 * Function is very similar to set_rgb, but sets doubles, checks
 * if values from token are normalized, calls vec3().
 */
static t_vec3	set_vec3(t_objects *obj, int index, char *func_name,
		int normalized)
{
	char	**coord_tok;
	double	coord_val[3];
	int		i;

	coord_tok = ft_split(obj->_tokens[index], ',');
	if (*coord_tok == NULL)
		call_error("ft_split() fail", func_name, obj);
	i = -1;
	while (++i < 3)
	{
		coord_val[i] = ft_atod(coord_tok[i]);
		// if (normalized && (coord_val[i] < -1.0 || coord_val[i] > 1.0))
		// {
		// 	free_split(coord_tok);
		// 	call_error("vector not normalized", func_name, obj);
		// }
	}
	free_split(coord_tok);
	return (vec3(coord_val[0], coord_val[1], coord_val[2]));
}

/* Inits the ambient struct inside of t_mrt->t_camera.
 * usage:
 * "A" [intensity([0.0;1.0])] [rgb color]
 */
static void	get_ambient(t_mrt *data)
{
	static int	already_set;
	char		**tokens;

	tokens = data->objects._tokens;
	if (already_set)
		call_error("this element can only be set once", "ambient",
			&data->objects);
	if (count_tokens(tokens) != 3)
		call_error("invalid token amount", "ambient", &data->objects);
	ambient(&data->cam.ambient, ft_atod(tokens[1]), set_rgb(&data->objects, 2,
			"ambient"));
	already_set = 1;
}

/* Inits the camera struct inside of t_mrt.
 * usage:

	* "C" [origin] [orientation(view) vector (normalized values)] [horiz. fow (double)]
 */
static void	get_camera(t_mrt *data)
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
	init_cam(&data->cam, set_vec3(&data->objects, 1, "camera", 0),
		set_vec3(&data->objects, 2, "camera", 1), hfov);
	already_set = 1;
}

/* Quad light.
 * usage:

	* "l" "qd" [origin] [side_vector1] [side_vector2] [rgb color] [intensity ([0.0;1.0])]
 */
static void	quad_light(t_objects *obj, int set_index)
{
	t_color	color;
	t_rgb	rgbcolor;

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
	quad_mat(&obj->lights[set_index].q_body, set_vec3(obj, 2, "q_light", 0),
		set_vec3(obj, 3, "q_light", 0), set_vec3(obj, 4, "q_light", 0),
		(t_material *)&obj->lights[set_index].difflight);
}

/* Makes a light object, which is a sphere or a quad with a light texture.
 * Adds the sphere from the light struct to the regular hittable list.
 * as well as the light one.
 */
/*
 * usage (for default sphere light):

	* "l" [origin] [intensity([0.0;1,0])] [rgb color] [optional : diameter(default value=100)]
 */
static void	get_light(t_objects *obj)
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

/*
 * Checks if a string represents a float number.
 * (the string can have just a single dot and digits)
 */
bool	is_float(char *str)
{
	int		i;
	bool	has_dot;

	has_dot = false;
	i = -1;
	while (str[++i])
	{
		if (ft_isdigit(str[i]) == false)
		{
			if (str[i] == '.' && has_dot == false)
				has_dot = true;
			else
				return (false);
		}
	}
	return (true);
}

/*
 * usage:
 * default sphere -		"sp" [origin] [diameter] [rgb color]
 * checker texture -	"sp" [origin] [diameter] [rgb color1] [rgb color2]
 * image (earthmap) -	"sp" [origin] [diameter] "img:"[path to .jpg]
 * metal sphere
			-		"sp" [origin] [diameter] [rgb color] [fuzz value(double)]
 */
static void	get_sphere(t_objects *obj)
{
	static int	set_index;
	char		**tokens;

	double diam; // fixed this - spheres less than 1 would be ignored otherwise
	tokens = obj->_tokens;
	if (set_index >= SPHERES_COUNT)
		call_error("exceeds array size", "sphere", obj);
	if (count_tokens(tokens) < 4)
		call_error("needs at least 4 tokens", "sphere", obj);
	diam = ft_atod(tokens[2]);
	if (diam < 0)
		call_error("diameter cannot be negative...", "sphere", obj);
	if (count_tokens(tokens) == 5 && is_float(tokens[4]))
	{
		metal_init(&obj->spheres[set_index].metal, set_rgb(obj, 3, "sphere"),
			ft_atod(tokens[4]));
		sphere_mat(&obj->spheres[set_index], set_vec3(obj, 1, "sphere", 0),
			diam, (t_material *)&obj->spheres[set_index].metal);
	}
	else if (count_tokens(tokens) == 5)
	{
		checker_texture_init(&obj->spheres[set_index].checker, CHECKER_SIZE,
			set_rgb(obj, 3, "sphere"), set_rgb(obj, 4, "sphere"));
		lambertian_init_tex(&obj->spheres[set_index].lambertian_mat,
			(t_texture *)&obj->spheres[set_index].checker);
		sphere_mat(&obj->spheres[set_index], set_vec3(obj, 1, "sphere", 0),
			diam, (t_material *)&obj->spheres[set_index].lambertian_mat);
	}
	else if (count_tokens(tokens) == 4 && ft_strncmp(tokens[3], "img:", 4) == 0)
	{
		img_texture_init(&obj->spheres[set_index].img_texture, &tokens[3][4]);
		lambertian_init_tex(&obj->spheres[set_index].lambertian_mat,
			(t_texture *)&obj->spheres[set_index].img_texture);
		sphere_mat(&obj->spheres[set_index], set_vec3(obj, 1, "sphere", 0),
			diam, (t_material *)&obj->spheres[set_index].lambertian_mat);
	}
	else
		sphere(&obj->spheres[set_index], set_vec3(obj, 1, "sphere", 0), diam,
			set_rgb(obj, 3, "sphere"));
	obj->hit_list[obj->hit_idx] = (t_hittable *)&obj->spheres[set_index];
	obj->hit_idx++;
	set_index++;
}

/*
 * usage:
 * regular plane
		-	"pl" [origin] [surface normal ([0;1],[0;1],[0;1])] [rgb color]
 * metal plane
		-	"pl" [origin] [surface normal ([0;1],[0;1],[0;1])] [rgb color] [fuzz(double)]
 * checker plane
		-	"pl" [origin] [surface normal ([0;1],[0;1],[0;1])] [rgb color1] [rgb color2]
 */
static void	get_plane(t_objects *obj)
{
	static int	set_index;
	char		**tokens;

	tokens = obj->_tokens;
	if (set_index >= OBJECT_COUNT)
		call_error("exceeds array size", "plane", obj);
	if (count_tokens(tokens) != 4 && count_tokens(tokens) != 5)
		call_error("invalid token amount", "plane", obj);
	if (count_tokens(tokens) == 5 && is_float(tokens[4]))
	{
		metal_init(&obj->planes[set_index].metal, set_rgb(obj, 3, "plane"),
			ft_atod(tokens[4]));
		plane_mat(&obj->planes[set_index], set_vec3(obj, 1, "plane", 0),
			set_vec3(obj, 2, "plane", 1),
			(t_material *)&obj->planes[set_index].metal);
	}
	else if (count_tokens(tokens) == 5)
	{
		checker_texture_init(&obj->planes[set_index].checker, CHECKER_SIZE,
			set_rgb(obj, 3, "plane"), set_rgb(obj, 4, "plane"));
		lambertian_init_tex(&obj->planes[set_index].lambertian_mat,
			(t_texture *)&obj->planes[set_index].checker);
		plane_mat(&obj->planes[set_index], set_vec3(obj, 1, "plane", 0),
			set_vec3(obj, 2, "plane", 1),
			(t_material *)&obj->planes[set_index].lambertian_mat);
	}
	else if (count_tokens(tokens) == 4)
	{
		plane(&obj->planes[set_index], set_vec3(obj, 1, "plane", 0),
			set_vec3(obj, 2, "plane", 1), set_rgb(obj, 3, "plane"));
	}
	obj->hit_list[obj->hit_idx] = (t_hittable *)&obj->planes[set_index];
	obj->hit_idx++;
	set_index++;
}

/*
 * usage:
 * default, capped cylinder
		-	"cy" [origin] [axis normal] [diameter] [height] [rgb color]
 * metalic, capped cylinder
		-	"cy" [origin] [axis normal] [diameter] [height] [rgb color] [fuzz(double)]
 */
static void	get_cylinder(t_objects *obj)
{
	static int	set_index;
	char		**tokens;

	tokens = obj->_tokens;
	if (set_index >= CYLINDER_COUNT)
		call_error("exceeds array size", "cylinder", obj);
	if (count_tokens(tokens) != 6 && count_tokens(tokens) != 7)
		call_error("invalid token amount", "cylinder", obj);
	if (count_tokens(tokens) == 7)
	{
		metal_init(&obj->cylinders[set_index].cylinder_uncapped.metal,
			set_rgb(obj, 3, "cylinder"), ft_atod(tokens[4]));
		cylinder_mat_capped(&obj->cylinders[set_index], set_vec3(obj, 1,
				"cylinder", 0), set_vec3(obj, 2, "cylinder", 1),
			ft_atod(tokens[3]), ft_atod(tokens[4]),
			(t_material *)&obj->cylinders[set_index].cylinder_uncapped.metal);
	}
	else
	{
		cylinder_capped(&obj->cylinders[set_index], set_vec3(obj, 1, "cylinder",
				0), set_vec3(obj, 2, "cylinder", 1), ft_atod(tokens[3]),
			ft_atod(tokens[4]), set_rgb(obj, 5, "cylinder"));
	}
	obj->hit_list[obj->hit_idx] = (t_hittable *)&obj->cylinders[set_index];
	obj->hit_idx++;
	set_index++;
}

/*
 * usage:
 * default, uncapped cylinder
		-	"cyu" [origin] [axis normal] [diameter] [height] [rgb color]
 * metalic, uncapped cylinder
		-	"cyu" [origin] [axis normal] [diameter] [height] [rgb color] [fuzz(double)]
 */
static void	get_cylinder_u(t_objects *obj)
{
	static int	set_index;
	char		**tokens;

	tokens = obj->_tokens;
	if (set_index >= CYLINDER_COUNT)
		call_error("exceeds array size", "cylinder_u", obj);
	if (count_tokens(tokens) != 6 && count_tokens(tokens) != 7)
		call_error("invalid token amount", "cylinde_u", obj);
	if (count_tokens(tokens) == 7)
	{
		metal_init(&obj->cylinders_u[set_index].metal,
			set_rgb(obj, 3, "cylinder_u"), ft_atod(tokens[4]));
		cylinder_mat_uncapped(&obj->cylinders_u[set_index], set_vec3(obj, 1,
				"cylinder_u", 0), set_vec3(obj, 2, "cylinder_u", 1),
			ft_atod(tokens[3]), ft_atod(tokens[4]),
			(t_material *)&obj->cylinders_u[set_index].metal);
	}
	else
	{
		cylinder_uncapped(&obj->cylinders_u[set_index], set_vec3(obj, 1, "cylinder_u",
				0), set_vec3(obj, 2, "cylinder_u", 1), ft_atod(tokens[3]),
			ft_atod(tokens[4]), set_rgb(obj, 5, "cylinder_u"));
	}
	obj->hit_list[obj->hit_idx] = (t_hittable *)&obj->cylinders_u[set_index];
	obj->hit_idx++;
	set_index++;
}
/*
 * usage:
 * default quad -	"qd" [origin] [side_vector1] [side_vector2] [color]
 * metalic quad
		-	"qd" [origin] [side_vector1] [side_vector2] [color] [fuzz(double)]
 * (for quad light check quad_light())
 */
/* TODO: cant get quad to render, might just be the coords */
static void	get_quad(t_objects *obj)
{
	static int	set_index;
	char		**tokens;

	tokens = obj->_tokens;
	if (set_index >= OBJECT_COUNT)
		call_error("exceeds array size", "quad", obj);
	if (count_tokens(tokens) != 5 && count_tokens(tokens) != 6)
		call_error("invalid token amount", "quad", obj);
	if (count_tokens(tokens) == 6)
	{
		metal_init(&obj->quads[set_index].metal, set_rgb(obj, 4, "quad"),
			ft_atod(tokens[4]));
		quad_mat(&obj->quads[set_index], set_vec3(obj, 1, "quad", 0),
			set_vec3(obj, 2, "quad", 0), set_vec3(obj, 3, "quad", 0),
			(t_material *)&obj->quads[set_index].metal);
	}
	else
	{
		quad_rgb(&obj->quads[set_index], set_vec3(obj, 1, "quad", 0),
			set_vec3(obj, 2, "quad", 0), set_vec3(obj, 3, "quad", 0),
			set_rgb(obj, 4, "quad"));
	}
	obj->hit_list[obj->hit_idx] = (t_hittable *)&obj->quads[set_index];
	obj->hit_idx++;
	set_index++;
}

/*
 * usage:
 * default -	"dsk" [origin] [surface normal] [diameter] [rgb color]
 * metalic
		-	"dsk" [origin] [surface normal] [diameter] [rgb color] [fuzz(double)]
 */
static void	get_disk(t_objects *obj)
{
	static int	set_index;
	char		**tokens;

	tokens = obj->_tokens;
	if (set_index >= OBJECT_COUNT)
		call_error("exceeds array size", "disk", obj);
	if (count_tokens(tokens) != 5 && count_tokens(tokens) != 6)
		call_error("invalid token amount", "disk", obj);
	if (count_tokens(tokens) == 6)
	{
		metal_init(&obj->disks[set_index].metal, set_rgb(obj, 4, "disk"),
			ft_atod(tokens[4]));
		disk_mat(&obj->disks[set_index], set_vec3(obj, 1, "disk", 0),
			set_vec3(obj, 2, "disk", 0), ft_atod(tokens[3]),
			(t_material *)&obj->disks[set_index].metal);
	}
	else
	{
		disk(&obj->disks[set_index], set_vec3(obj, 1, "disk", 0), set_vec3(obj,
				2, "disk", 1), ft_atod(tokens[3]), set_rgb(obj, 4, "disk"));
	}
	obj->hit_list[obj->hit_idx] = (t_hittable *)&obj->disks[set_index];
	obj->hit_idx++;
	set_index++;
}

/*
 * usage:
 * default -	"tr" [vertice1] [vertice2] [vertice3] [rbg color]
 * metalic -	"tr" [vertice1] [vertice2] [vertice3] [rbg color] [fuzz(double)]
 * (vertices are points in 3d space)
 */
/* TODO: might not be rendering - the same as quad*/
static void	get_triangle(t_objects *obj)
{
	static int	set_index;
	char		**tokens;

	tokens = obj->_tokens;
	if (set_index >= OBJECT_COUNT)
		call_error("exceeds array size", "triangle", obj);
	if (count_tokens(tokens) != 5 && count_tokens(tokens) != 6)
		call_error("invalid token amount", "triangle", obj);
	if (count_tokens(tokens) == 6)
	{
		metal_init(&obj->triangles[set_index].metal, set_rgb(obj, 4,
				"triangle"), ft_atod(tokens[5]));
		triangle_mat(&obj->triangles[set_index], set_vec3(obj, 1, "triangle",
				0), set_vec3(obj, 2, "triangle", 0), set_vec3(obj, 3,
				"triangle", 0), (t_material *)&obj->triangles[set_index].metal);
	}
	else
	{
		triangle(&obj->triangles[set_index], set_vec3(obj, 1, "triangle", 0),
			set_vec3(obj, 2, "triangle", 0), set_vec3(obj, 3, "triangle", 0),
			set_rgb(obj, 4, "triangle"));
	}
	obj->hit_list[obj->hit_idx] = (t_hittable *)&obj->triangles[set_index];
	obj->hit_idx++;
	set_index++;
}

/*
 * usage:
 * default - "co" [apex] [axis] [diam] [height] [color]
 * metalic - "co" [apex] [axis] [diam] [height] [color] [fuzz]
 *  
 * apex - the pointy end of the cone
 * axis - axis of cone (goes up from the apex)
 * diam - the diameter of the base of the cone
*/
static void	get_cone (t_objects *obj)
{
	static int	set_index;
	char		**tokens;

	tokens = obj->_tokens;
	if (set_index >= OBJECT_COUNT)
		call_error("exceeds array size", "cone", obj);
	if (count_tokens(tokens) != 6 && count_tokens(tokens) != 7)
		call_error("invalid token amount", "cone", obj);
	if (count_tokens(tokens) == 7)
	{
		metal_init(&obj->cones[set_index].body.metal, set_rgb(obj, 5, "cone"), ft_atod(tokens[6]));
		cone_mat(&obj->cones[set_index], set_vec3(obj, 1, "cone", 0), set_vec3(obj, 2, "cone", 1), \
			ft_atod(tokens[3]), ft_atod(tokens[4]), (t_material*)&obj->cones[set_index].body.metal);
	}
	else
	{
		cone_rgb(&obj->cones[set_index], set_vec3(obj, 1, "cone", 0), set_vec3(obj, 2, "cone", 1), \
			ft_atod(tokens[3]), ft_atod(tokens[4]), set_rgb(obj, 5, "cone"));
	}
	obj->hit_list[obj->hit_idx] = (t_hittable *)&obj->cones[set_index];
	obj->hit_idx++;
	set_index++;
}

/*
 * The UNCAPPED cone
 * usage:
 * default - "co_u" [apex] [axis] [diam] [height] [color]
 * metalic - "co_u" [apex] [axis] [diam] [height] [color] [fuzz]
 *  
 * apex - the pointy end of the cone
 * axis - axis of cone (goes up from the apex)
 * diam - the diameter of the base of the cone
*/
static void	get_cone_u (t_objects *obj)
{
	static int	set_index;
	char		**tokens;

	tokens = obj->_tokens;
	if (set_index >= OBJECT_COUNT)
		call_error("exceeds array size", "cone_u", obj);
	if (count_tokens(tokens) != 6 && count_tokens(tokens) != 7)
		call_error("invalid token amount", "cone_u", obj);
	if (count_tokens(tokens) == 7)
	{
		metal_init(&obj->cones_u[set_index].metal, set_rgb(obj, 5, "cone_u"), ft_atod(tokens[6]));
		cone_uncap_mat(&obj->cones_u[set_index], set_vec3(obj, 1, "cone_u", 0), set_vec3(obj, 2, "cone_u", 1), \
			ft_atod(tokens[3]), ft_atod(tokens[4]), (t_material*)&obj->cones_u[set_index].metal);
	}
	else
	{
		cone_uncap_rgb(&obj->cones_u[set_index], set_vec3(obj, 1, "cone_u", 0), set_vec3(obj, 2, "cone_u", 1), \
			ft_atod(tokens[3]), ft_atod(tokens[4]), set_rgb(obj, 5, "cone_u"));
	}
	obj->hit_list[obj->hit_idx] = (t_hittable *)&obj->cones_u[set_index];
	obj->hit_idx++;
	set_index++;
}
/*
 * usage:
 * default - "box" [origin] [diagonal point] [color]
 * metalic - "box" [origin] [diagonal point] [color] [fuzz(double)]
 */
static void	get_box(t_objects *obj)
{
	static int	set_index;
	char		**tokens;

	tokens = obj->_tokens;
	if (set_index >= OBJECT_COUNT)
		call_error("exceeds array size", "box", obj);
	if (count_tokens(tokens) != 4 && count_tokens(tokens) != 5)
		call_error("invalid token amount", "box", obj);
	if (count_tokens(tokens) == 5)
	{
		metal_init(&obj->boxes[set_index].metal, set_rgb(obj, 3, "box"),
			ft_atod(tokens[4]));
		box(&obj->boxes[set_index], set_vec3(obj, 1, "box", 0), set_vec3(obj, 2,
				"box", 0), (t_material *)&obj->boxes[set_index].metal);
	}
	else
	{
		box_rgb(&obj->boxes[set_index], set_vec3(obj, 1, "box", 0),
			set_vec3(obj, 2, "box", 0), set_rgb(obj, 3, "box"));
	}
	obj->hit_list[obj->hit_idx] = (t_hittable *)&obj->boxes[set_index];
	obj->hit_idx++;
	set_index++;
}

static void	update_struct(t_mrt *data)
{
	if (ft_strncmp("C", data->objects._tokens[0], 2) == 0)
		get_camera(data);
	else if (ft_strncmp("A", data->objects._tokens[0], 2) == 0)
		get_ambient(data);
	else if (ft_strncmp("l", data->objects._tokens[0], 2) == 0
		|| ft_strncmp("L", data->objects._tokens[0], 2) == 0)
		get_light(&data->objects);
	else if (ft_strncmp("sp", data->objects._tokens[0], 3) == 0)
		get_sphere(&data->objects);
	else if (ft_strncmp("pl", data->objects._tokens[0], 3) == 0)
		get_plane(&data->objects);
	else if (ft_strncmp("cy", data->objects._tokens[0], 3) == 0)
		get_cylinder(&data->objects);
	else if (ft_strncmp("cy_u", data->objects._tokens[0], 3) == 0)
		get_cylinder_u(&data->objects);
	else if (ft_strncmp("qd", data->objects._tokens[0], 3) == 0)
		get_quad(&data->objects);
	else if (ft_strncmp("dsk", data->objects._tokens[0], 4) == 0)
		get_disk(&data->objects);
	else if (ft_strncmp("tr", data->objects._tokens[0], 3) == 0)
		get_triangle(&data->objects);
	else if (ft_strncmp("box", data->objects._tokens[0], 4) == 0)
		get_box(&data->objects);
	else if (ft_strncmp("co", data->objects._tokens[0], 3) == 0)
		get_cone(&data->objects);
	else if (ft_strncmp("co_u", data->objects._tokens[0], 3) == 0)
		get_cone_u(&data->objects);
	else
		call_error("invalid object identifier", data->objects._tokens[0],
			&data->objects);
}

/* replaces tabs and newlines, so thta ft_split can split
on just the space. removes comments as well */
static void	sanitize_line(char *line)
{
	int	i;

	i = -1;
	while (line[++i])
	{
		if (line[i] == '\t' || line[i] == '\n')
			line[i] = ' ';
		else if (line[i] == '#')
		{
			line[i] = ' ';
			line[++i] = '\0';
			return ;
		}
	}
}

/*
 * The light struct will always have something in it.
 * If there are no lights in the input, then this "nothing light"
 * will be the only thing passed to the light hittable list.
 * It will not do anything, but without it ambient light doesnt seem to work.
 *
 * If there are light int the input, then the "nothing light" will be
 * written over and nonexistant.
 */
static void	init_light_struct(t_mrt *data)
{
	t_empty_material	empty_material;

	quad_mat(&data->objects.lights[0].q_body, point3(0.1, 0.1, 0.1), vec3(0.1,
			1, 0.1), vec3(1, 0.1, 0.1), (t_material *)&empty_material);
	data->objects.light_hit[0] = (t_hittable *)&data->objects.lights[0].q_body;
	data->objects.light_hit_idx++;
}

static bool	ft_isspace(char *str)
{
	int	i;

	i = -1;
	while (str[++i])
		if (str[i] != ' ')
			return (false);
	return (true);
}

/* in case or error, the parser calls exit() */
void	parse_input(char *filename, t_mrt *data)
{
	char	*line;

	if (ft_strncmp(&filename[ft_strlen(filename) - 3], ".rt", 3) != 0)
		call_error("invalid file extension", NULL, NULL);
	data->objects._file_fd = open(filename, O_RDONLY);
	if (data->objects._file_fd == -1)
		perror(filename), exit(1);
	init_light_struct(data);
	while ((line = get_next_line(data->objects._file_fd)) != NULL)
	{
		sanitize_line(line);
		if (ft_strlen(line) == 1 || ft_isspace(line) == true)
		{
			free(line);
			continue ;
		}
		data->objects._tokens = ft_split(line, ' ');
		if (data->objects._tokens == NULL)
		{
			free(line);
			call_error("ft_split()", "parse_input", &data->objects);
		}
		free(line);
		update_struct(data);
		free_split(data->objects._tokens);
	}
	data->objects._tokens = NULL;
	if (data->cam.aspect_ratio == 0)
		call_error("There has to be a camera object!!!", "parse_input",
			&data->objects);
	data->world = hittablelist(data->objects.hit_list, data->objects.hit_idx);
	data->lights = hittablelist(data->objects.light_hit,
			data->objects.light_hit_idx);
	if (close(data->objects._file_fd) == -1)
		perror("MiniRT: close()");
}
