/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ralgaran <ralgaran@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 21:22:36 by ralgaran          #+#    #+#             */
/*   Updated: 2024/09/06 17:20:25 by ralgaran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"  // get_next_line should be here
#include <fcntl.h>  /* open() */
#include <stdio.h>  /* perror() */
#include "minirt.h"

/* program will exit if there is an error */
static int call_error(char *msg, char *prefix)
{
	write(2, "Error\n", 6);
	if (prefix)
	{
		write(2, prefix, ft_strlen(prefix));
		write(2, ": ", 2);
	}
	write(2, msg, ft_strlen(msg));
	write(2, "\n", 1);

	// TODO: may need to close open file first
	exit(1);
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
static t_rgb	set_rgb(char *token, char *func_name)
{
	char	**rgb_tok;
	int		rgb_val[3];
	int		i;

	rgb_tok = ft_split(token, ',');
	if (*rgb_tok == NULL)
		call_error("ft_split() fail", func_name);
	i = -1;
	while (++i < 3)
	{
		rgb_val[i] = ft_atoi(rgb_tok[i]);
		if (rgb_val[i] < 0 || rgb_val[i] > 255)
			call_error("color not in rgb range", func_name);
	}
	free(rgb_tok);
	return(rgb(rgb_val[0], rgb_val[1], rgb_val[2]));
}

/*
 * Returns t_vec3 with values from the token.
 *
 * Function is very similar to set_rgb, but sets doubles, checks
 * if values from token are normalized, calls vec3().
*/
static t_vec3	set_vec3(char *token, char *func_name, int normalized)
{
	char	**coord_tok;
	double	coord_val[3];
	int		i;

	coord_tok = ft_split(token, ',');
	if (*coord_tok == NULL)
		call_error("ft_split() fail", func_name);
	i = -1;
	while (++i < 3)
	{
		coord_val[i] = ft_atod(coord_tok[i]);
		if (normalized && (coord_val[i] < .0 || coord_val[i] > 1.0))
			call_error("vector not normalized", func_name);
	}
	free(coord_tok);
	return(vec3(coord_val[0], coord_val[1], coord_val[2]));
}

static void	get_ambient(char **tokens, t_ambient *data)
{
	static int	already_set;

	if (already_set)
		call_error("this element can only be set once", "ambient");
	if (count_tokens(tokens) != 3)
		call_error("invalid token amount", "ambient");
	data->ratio = ft_atod(tokens[1]);
	data->rgbcolor = set_rgb(tokens[2], "ambient");
	already_set = 1;
}

static void	get_camera(char **tokens, t_camera *data)
{
	static int	already_set;

	if (already_set)
		call_error("this element can only be set once", "camera");
	if (count_tokens(tokens) != 4)
		call_error("invalid token amount", "camera");
	data->center = set_vec3(tokens[1], "camera", 0);
	data->direction = set_vec3(tokens[2], "camera", 1);
	data->hfov = ft_atod(tokens[3]);
	if (data->hfov < 0. || data->hfov > 180.)
		call_error("fov must be in range [0;180]", "camera");
	already_set = 1;
}

static void	get_light(char **tokens, t_light *data)
{
	static int	already_set;
	int			token_count;

	if (already_set)
		call_error("this element can only be set once", "light");
	token_count = count_tokens(tokens);
	if (token_count < 3 || token_count > 4)
		call_error("invalid token amount", "light");
	data->center = set_vec3(tokens[1], "light", 0);
	data->brightness = ft_atod(tokens[2]);
	if (data->brightness < 0. || data->brightness > 1.)
		call_error("brightness must be normalized", "light");
	data->color = set_rgb(tokens[3], "color");
	already_set = 1;
}

static void	get_sphere(char **tokens, t_sphere *spheres)
{
	static int	set_index;

	if (set_index >= OBJECT_COUNT)
		call_error("exceeds array size", "sphere");
	spheres[set_index].center = set_vec3(tokens[1], "sphere", 0);
	spheres[set_index].radius = ft_atod(tokens[2]) / 2.;
	spheres[set_index].rgb = set_rgb(tokens[3], "sphere");
	set_index++;
}

static void	get_plane(char **tokens, t_plane *planes)
{
	static int	set_index;

	if (set_index >= OBJECT_COUNT)
		call_error("exceeds array size", "plane");
	planes[set_index].point = set_vec3(tokens[1], "plane", 0);
	planes[set_index].normal = set_vec3(tokens[2], "plane", 1);
	planes[set_index].rgb = set_rgb(tokens[3], "plane");
	set_index++;
}

static void	get_cylinder(char **tokens, t_cylinder *cylinders)
{
	static int	set_index;

	if (set_index >= OBJECT_COUNT)
		call_error("exceeds array size", "cylinder");
	cylinders[set_index].center = set_vec3(tokens[1], "cylinder", 0);
	cylinders[set_index].axis = set_vec3(tokens[2], "cylinder", 1);
	cylinders[set_index].radius = ft_atod(tokens[3]) / 2.;
	cylinders[set_index].height = ft_atod(tokens[4]);
	cylinders[set_index].rgb = set_rgb(tokens[5], "cylinder");
	set_index++;
}

static int	update_struct(t_objects *obj, char **tokens)
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
		return(call_error("invalid object identifier\n", tokens[1]));
	return (0);
}

/* in case or error, the parser calls exit() */
void	parse_input(char *filename, t_objects *obj)
{
    int	    fd;
    char    *line;
    char    **tokens;

    if (ft_strncmp(&filename[ft_strlen(filename) - 3], ".rt", 3) != 0)
		call_error("invalid file extension\n", NULL);
    fd = open(filename, O_RDONLY);
    if (fd == -1)
    	perror(filename), exit(1);
    while ((line = get_next_line(fd)) != NULL)
    {
		if (ft_strlen(line) == 1)
		    continue ;
		tokens = ft_split(line, ' ');
		if (tokens == NULL)
			call_error("ft_split()", "parse_input");
		update_struct(obj, tokens);
		free(tokens);
    }
    if (close(fd) == -1)
    	perror("MiniRT: close()");
}
