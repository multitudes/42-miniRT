/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 21:22:36 by ralgaran          #+#    #+#             */
/*   Updated: 2024/09/10 18:53:41 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"  // get_next_line should be here
#include <fcntl.h>  /* open() */
#include <math.h>
#include <stdio.h>  /* perror() */
#include "minirt.h"
#include <assert.h>

static void	free_split(char **split)
{
	int	i;

	i = -1;
	while (split[++i])
		free(split[i]);
	free(split);
}

/* program will exit if there is an error */
static int call_error(char *msg, char *prefix, char **tokens)
{
	write(2, "Error\n", 6);
	if (prefix)
	{
		write(2, prefix, ft_strlen(prefix));
		write(2, ": ", 2);
	}
	write(2, msg, ft_strlen(msg));
	write(2, "\n", 1);
	if (tokens)
		free_split(tokens);
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
static t_rgb	set_rgb(char **tokens, int index, char *func_name)
{
	char	**rgb_tok;
	int		rgb_val[3];
	int		i;

	rgb_tok = ft_split(tokens[index], ',');
	if (*rgb_tok == NULL)
		call_error("ft_split() fail", func_name, tokens);
	i = -1;
	while (++i < 3)
	{
		rgb_val[i] = ft_atoi(rgb_tok[i]);
		if (rgb_val[i] < 0 || rgb_val[i] > 255)
		{
			free_split(rgb_tok);
			call_error("color not in rgb range", func_name, tokens);
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
static t_vec3	set_vec3(char **tokens, int index, char *func_name, int normalized)
{
	char	**coord_tok;
	double	coord_val[3];
	int		i;

	coord_tok = ft_split(tokens[index], ',');
	if (*coord_tok == NULL)
		call_error("ft_split() fail", func_name, tokens);
	i = -1;
	while (++i < 3)
	{
		coord_val[i] = ft_atod(coord_tok[i]);
		if (normalized && (coord_val[i] < .0 || coord_val[i] > 1.0))
		{
			free_split(coord_tok);
			call_error("vector not normalized", func_name, tokens);
		}
	}
	free_split(coord_tok);
	return(vec3(coord_val[0], coord_val[1], coord_val[2]));
}

static void	get_ambient(char **tokens, t_ambient *data)
{
	static int	already_set;

	if (already_set)
		call_error("this element can only be set once", "ambient", tokens);
	if (count_tokens(tokens) != 3)
		call_error("invalid token amount", "ambient", tokens);
	*data = ambient(ft_atod(tokens[1]), set_rgb(tokens, 2, "ambient"));
	already_set = 1;
}

static void	get_camera(char **tokens, t_camera *data)
{
	static int	already_set;
	double		hfov;

	if (already_set)
		call_error("this element can only be set once", "camera", tokens);
	if (count_tokens(tokens) != 4)
		call_error("invalid token amount", "camera", tokens);
	hfov = ft_atod(tokens[3]);
	if (hfov < 0. || hfov > 180.)
		call_error("fov must be in range [0;180]", "camera", tokens);
	*data = init_cam(set_vec3(tokens, 1, "camera", 0), \
		set_vec3(tokens, 2, "camera", 1), hfov);
	already_set = 1;
}

static void	get_light(char **tokens, t_light *data)
{
	static int	already_set;
	int			token_count;

	if (already_set)
		call_error("this element can only be set once", "light", tokens);
	token_count = count_tokens(tokens);
	if (token_count < 3 || token_count > 4)
		call_error("invalid token amount", "light", tokens);
	data->center = set_vec3(tokens, 1, "light", 0);
	data->brightness = ft_atod(tokens[2]);
	if (data->brightness < 0. || data->brightness > 1.)
		call_error("brightness must be normalized", "light", tokens);
	if (token_count == 4)
		data->color = set_rgb(tokens, 3, "color");
	already_set = 1;
}

static void	get_sphere(char **tokens, t_objects *obj)
{
	static int	set_index;

	if (set_index >= OBJECT_COUNT)
		call_error("exceeds array size", "sphere", tokens);
	obj->spheres[set_index] = sphere(set_vec3(tokens, 1, "sphere", 0), \
	 ft_atod(tokens[2]), set_rgb(tokens, 3, "sphere"));
	obj->hittable_list[obj->list_idx++] = (t_hittable*)&spheres[set_index];
	set_index++;
}

static void	get_plane(char **tokens, t_plane *planes)
{
	static int	set_index;

	if (set_index >= OBJECT_COUNT)
		call_error("exceeds array size", "plane", tokens);
	planes[set_index] = plane(set_vec3(tokens, 1, "plane", 0), \
		set_vec3(tokens, 2, "plane", 1), set_rgb(tokens, 3, "plane"));
	set_index++;
}

static void	get_cylinder(char **tokens, t_cylinder *cylinders)
{
	static int	set_index;

	if (set_index >= OBJECT_COUNT)
		call_error("exceeds array size", "cylinder", tokens);
	cylinders[set_index] = cylinder(set_vec3(tokens, 1, "cylinder", 0), \
	 set_vec3(tokens, 2, "cylinder", 1), ft_atod(tokens[3]), ft_atod(tokens[4]),
	 set_rgb(tokens, 5, "cylinder"));
	set_index++;
}

static void	update_struct(t_objects *obj, char **tokens)
{
	if (ft_strncmp("A", tokens[0], 2) == 0)
		get_ambient(tokens, &obj->ambient);
	else if (ft_strncmp("C", tokens[0], 2) == 0)
		get_camera(tokens, &obj->camera);
	else if (ft_strncmp("L", tokens[0], 2) == 0)
		get_light(tokens, &obj->light);
	else if (ft_strncmp("sp", tokens[0], 3) == 0)
		get_sphere(tokens, obj->spheres);
	else if (ft_strncmp("pl", tokens[0], 3) == 0)
		get_plane(tokens, obj->planes);
	else if (ft_strncmp("cy", tokens[0], 3) == 0)
		get_cylinder(tokens, obj->cylinders);
	else
		call_error("invalid object identifier\n", tokens[0], tokens);
}

/* in case or error, the parser calls exit() */
void	parse_input(char *filename, t_objects *obj)
{
    int	    fd;
	char    *line;
    char    **tokens;
	

    if (ft_strncmp(&filename[ft_strlen(filename) - 3], ".rt", 3) != 0)
		call_error("invalid file extension\n", NULL, NULL);
    fd = open(filename, O_RDONLY);
    if (fd == -1)
    	perror(filename), exit(1);
    while ((line = get_next_line(fd)) != NULL)
    {
		if (ft_strlen(line) == 1)
		{
			free(line);
			continue ;
		}
		tokens = ft_split(line, ' ');
		if (tokens == NULL)
		{
			free(line);
			call_error("ft_split()", "parse_input", NULL);
		}
		free(line);
		update_struct(obj, tokens);
		free_split(tokens);
    }
    if (close(fd) == -1)
    	perror("MiniRT: close()");
}
