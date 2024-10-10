/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 16:42:10 by ralgaran          #+#    #+#             */
/*   Updated: 2024/10/10 19:29:58 by lbrusa           ###   ########.fr       */
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
bool	is_float(char *str);
void	sanitize_line(char *line);
bool	ft_isspace(char *str);

/* parse_obj1.c */
void	get_ambient(t_mrt *data);
void	get_camera(t_mrt *data);
void	get_light(t_objects *obj);

/* parse_obj2.c */
void	get_sphere(t_objects *obj);
void	get_plane(t_objects *obj);

/* parse_obj3.c */
void	get_cylinder(t_objects *obj);
void	get_cylinder_u(t_objects *obj);
void	get_quad(t_objects *obj);

/* parse_obj4.c */
void	get_disk(t_objects *obj);
void	get_triangle(t_objects *obj);
void	get_box(t_objects *obj);

/* parse_obj5.c */
void	get_cone(t_objects *obj);
void	get_cone_u(t_objects *obj);

/* parse_obj6.c */
void	quad_light_empty(t_objects *obj, int set_index);
void	sphere_light_empty(t_objects *obj, int set_index);

#endif
