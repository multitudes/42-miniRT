/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ralgaran <ralgaran@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 16:42:10 by ralgaran          #+#    #+#             */
/*   Updated: 2024/10/03 16:42:13 by ralgaran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_H
# define PARSE_H

# include "libft.h"  	// get_next_line should be here
# include <fcntl.h>  	/* open() */
# include <stdio.h>  	/* perror() */
# include "minirt.h"

/* parse_utils1.c */
void	free_split(char **split);
int		call_error(char *msg, char *prefix, t_objects *obj);
int		count_tokens(char **tokens);
double	ft_atod(char *str);

/* parse_utils2.c */
t_rgb	set_rgb(t_objects *obj, int index, char *func_name);
t_vec3	set_vec3(t_objects *obj, int index, char *func_name, int normalized);

/* parse_obj1.c */
void	get_ambient(t_mrt *data);
void	get_camera(t_mrt *data);
void	quad_light(t_objects *obj, int set_index);
void	get_light(t_objects *obj);






#endif
