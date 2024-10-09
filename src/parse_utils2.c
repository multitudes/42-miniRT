/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 16:47:01 by ralgaran          #+#    #+#             */
/*   Updated: 2024/10/09 17:09:57 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

/* Returns a t_rgb struct that contains values from the token */
t_rgb	set_rgb(t_objects *obj, int index, char *func_name)
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
t_vec3	set_vec3(t_objects *obj, int index, char *func_name, int normalized)
{
	char	**coord_tok;
	double	coord_val[3];
	int		i;
	(void)normalized;

	coord_tok = ft_split(obj->_tokens[index], ',');
	if (*coord_tok == NULL)
		call_error("ft_split() fail", func_name, obj);
	i = -1;
	while (++i < 3)
		coord_val[i] = ft_atod(coord_tok[i]);
	free_split(coord_tok);
	return (vec3(coord_val[0], coord_val[1], coord_val[2]));
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

/* replaces tabs and newlines, so thta ft_split can split
on just the space. removes comments as well */
void	sanitize_line(char *line)
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

bool	ft_isspace(char *str)
{
	int	i;

	i = -1;
	while (str[++i])
		if (str[i] != ' ')
			return (false);
	return (true);
}
