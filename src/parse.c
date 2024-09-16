/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 21:22:36 by ralgaran          #+#    #+#             */
/*   Updated: 2024/09/16 17:38:27 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"  // get_next_line should be here
#include <fcntl.h>  /* open() */
#include <stdio.h>  /* perror() */
#include "minirt.h"

static void	free_split(char **split)
{
	int	i;

	i = -1;
	while (split[++i])
		free(split[i]);
	free(split);
}

/* program will exit if there is an error */
static int call_error(char *msg, char *prefix, t_objects *obj)
{
	write(2, "Error\n", 6);
	if (prefix)
	{
		write(2, prefix, ft_strlen(prefix));
		write(2, ": ", 2);
	}
	write(2, msg, ft_strlen(msg));
	write(2, "\n", 1);
	if (obj)
	{
		if (obj->_tokens)
			free_split(obj->_tokens);
		close(obj->_file_fd);
	}
	exit(1);
	// TODO: may need to close open file first
}

static int	count_tokens(char **tokens)
{
	int	i;

	i = -1;
	while (tokens[++i])
		;
	return (i);
}

/* converts a string to a double */
double	ft_atod(char *str)
{
	int			i;
	int			is_negative;
	long long	num;
	int			exponent;
	double		result;

	if (!str)
		return (.0);
	is_negative = 0;
	if (*str == '-')
	{
		is_negative = 1;
		str++;
	}
	num = 0;
	exponent = -1;
	i = -1;
	while (str[++i])
	{
		if (str[i] == '.')
		{
			if (exponent != -1 || ft_isdigit(str[i + 1]) == 0)
				break ;
			exponent = i;	// use the variable to store dot index;
			str++;
		}
		else if (ft_isdigit(str[i]) == 0)
			break ;
		num = (num * 10) + (int)(str[i] - '0');
	}
	result = (double) num;
	if (exponent != -1 && num != 0)
	{
		exponent = i - exponent;
		while (exponent--)
			result /= 10.;
	}
	if (is_negative)
		return (-result);
	return (result);
}

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
	return(rgb(rgb_val[0], rgb_val[1], rgb_val[2]));
}

/*
 * Returns t_vec3 with values from the token.
 *
 * Function is very similar to set_rgb, but sets doubles, checks
 * if values from token are normalized, calls vec3().
*/
static t_vec3	set_vec3(t_objects *obj, int index, char *func_name, int normalized)
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
		if (normalized && (coord_val[i] < -1.0 || coord_val[i] > 1.0))
		{
			free_split(coord_tok);
			call_error("vector not normalized", func_name, obj);
		}
	}
	free_split(coord_tok);
	return(vec3(coord_val[0], coord_val[1], coord_val[2]));
}

static void	get_ambient(t_mrt *data)
{
	static int	already_set;
	char		**tokens;

	tokens = data->objects._tokens;
	if (already_set)
		call_error("this element can only be set once", "ambient", &data->objects);
	if (count_tokens(tokens) != 3)
		call_error("invalid token amount", "ambient", &data->objects);
	ambient(&data->camera.ambient, ft_atod(tokens[1]), set_rgb(&data->objects, 2, "ambient"));
	already_set = 1;
}

static void	get_camera(t_mrt *data)
{
	static int	already_set;
	double		hfov;
	char		**tokens;

	tokens = data->objects._tokens;
	if (already_set)
		call_error("this element can only be set once", "camera", &data->objects);
	if (count_tokens(tokens) != 4)
		call_error("invalid token amount", "camera", &data->objects);
	hfov = ft_atod(tokens[3]);
	if (hfov < 0. || hfov > 180.)
		call_error("fov must be in range [0;180]", "camera", &data->objects);
	init_cam(&data->camera, set_vec3(&data->objects, 1, "camera", 0), \
		set_vec3(&data->objects, 2, "camera", 1), hfov);
	already_set = 1;
}

/*
 * Puts the light into the t_light array and the pointer to it
 * into the hittable_list.
 *
 * This function is longer than others, because light didnrt have an init func.
*/
/* TODO: init function? */
static void	get_light(t_objects *obj)
{
	static int	set_index;
	char		**tokens;

	tokens = obj->_tokens;
	if (count_tokens(tokens) != 4)
		call_error("invalid token amount", "light", obj);
	obj->lights[set_index].center = set_vec3(obj, 1, "light", 0);
	obj->lights[set_index].brightness = ft_atod(tokens[2]);
	if (obj->lights[set_index].brightness < 0. || obj->lights[set_index].brightness > 1.)
		call_error("brightness must be normalized", "light", obj);
	obj->lights[set_index].color = set_rgb(obj, 3, "color");
	obj->hit_list[obj->hit_idx] = (t_hittable *)&obj->spheres[set_index];
	obj->hit_idx++;
	set_index++;
}

static void	get_sphere(t_objects *obj)
{
	static int	set_index;
	char		**tokens;

	tokens = obj->_tokens;
	if (set_index >= OBJECT_COUNT)
		call_error("exceeds array size", "sphere", obj);
	if (count_tokens(tokens) != 4)
		call_error("invalid token amount", "light", obj);
	sphere(&obj->spheres[set_index], set_vec3(obj, 1, "sphere", 0), \
		ft_atod(tokens[2]), set_rgb(obj, 3, "sphere"));
	obj->hit_list[obj->hit_idx] = (t_hittable*)&obj->spheres[set_index];
	obj->hit_idx++;
	set_index++;
}

static void	get_plane(t_objects *obj)
{
	static int	set_index;
	char		**tokens;

	tokens = obj->_tokens;
	if (set_index >= OBJECT_COUNT)
		call_error("exceeds array size", "plane", obj);
	if (count_tokens(tokens) != 4)
		call_error("invalid token amount", "plane", obj);
	plane(&obj->planes[set_index], set_vec3(obj, 1, "plane", 0), \
		set_vec3(obj, 2, "plane", 1), set_rgb(obj, 3, "plane"));
	obj->hit_list[obj->hit_idx] = (t_hittable *)&obj->planes[set_index];
	obj->hit_idx++;
	set_index++;
}

static void	get_cylinder(t_objects *obj)
{
	static int	set_index;
	char		**tokens;

	tokens = obj->_tokens;
	if (set_index >= OBJECT_COUNT)
		call_error("exceeds array size", "cylinder", obj);
	if (count_tokens(tokens) != 6)
		call_error("invalid token amount", "cylinder", obj);
	cylinder_u(&obj->cylinders[set_index], set_vec3(obj, 1, "cylinder", 0), \
		set_vec3(obj, 2, "cylinder", 1), ft_atod(tokens[3]), ft_atod(tokens[4]), \
		set_rgb(obj, 5, "cylinder"));
	obj->hit_list[obj->hit_idx] = (t_hittable *)&obj->cylinders[set_index];
	obj->hit_idx++;
	set_index++;
}

static void	update_struct(t_mrt *data)
{
	if (ft_strncmp("A", data->objects._tokens[0], 2) == 0)
		get_ambient(data);
	else if (ft_strncmp("C", data->objects._tokens[0], 2) == 0)
		get_camera(data);
	else if (ft_strncmp("L", data->objects._tokens[0], 2) == 0)
		get_light(&data->objects);
	else if (ft_strncmp("sp", data->objects._tokens[0], 3) == 0)
		get_sphere(&data->objects);
	else if (ft_strncmp("pl", data->objects._tokens[0], 3) == 0)
		get_plane(&data->objects);
	else if (ft_strncmp("cy", data->objects._tokens[0], 3) == 0)
		get_cylinder(&data->objects);
	else
		call_error("invalid object identifier\n", data->objects._tokens[0], \
			&data->objects);
}

/* replaces tabs and newlines, so thta ft_split can split
on just the space */
void	sanitize_line(char *line)
{
	int	i;

	i = -1;
	while (line[++i])
	{
		if (line[i] == '\t' || line[i] == '\n')
			line[i] = ' ';
	}
}

/* in case or error, the parser calls exit() */
void	parse_input(char *filename, t_mrt *data)
{
	char    *line;

    if (ft_strncmp(&filename[ft_strlen(filename) - 3], ".rt", 3) != 0)
		call_error("invalid file extension\n", NULL, NULL);
    data->objects._file_fd = open(filename, O_RDONLY);
    if (data->objects._file_fd == -1)
    	perror(filename), exit(1);
    while ((line = get_next_line(data->objects._file_fd)) != NULL)
    {
		if (ft_strlen(line) == 1)
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
    if (close(data->objects._file_fd) == -1)
    	perror("MiniRT: close()");
}
