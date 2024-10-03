/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ralgaran <ralgaran@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 16:47:01 by ralgaran          #+#    #+#             */
/*   Updated: 2024/10/03 16:47:09 by ralgaran         ###   ########.fr       */
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
 t_vec3	set_vec3(t_objects *obj, int index, char *func_name,
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
		if (normalized && (coord_val[i] < -1.0 || coord_val[i] > 1.0))
		{
			free_split(coord_tok);
			call_error("vector not normalized", func_name, obj);
		}
	}
	free_split(coord_tok);
	return (vec3(coord_val[0], coord_val[1], coord_val[2]));
}
