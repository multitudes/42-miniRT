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

static void	update_struct_pt2(t_mrt *data)
{
	if (ft_strncmp("dsk", data->objects._tokens[0], 4) == 0)
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
	else
		update_struct_pt2(data);
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
	t_init_params		params;

	params.center = point3(0, 0, 0);
	params.diam = 10;
	params.mat = (t_material *)&empty_material;
	sphere_mat(&data->objects.lights[0].s_body, params);
	data->objects.light_hit[0] = (t_hittable *)&data->objects.lights[0].q_body;
	data->objects.light_idx++;
}

static void	parse_line_loop(t_mrt *data)
{
	char	*line;

	while (1)
	{
		line = get_next_line(data->objects._file_fd);
		if (line == NULL)
			break ;
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
}

/* in case or error, the parser calls exit() */
void	parse_input(char *filename, t_mrt *data)
{
	if (ft_strncmp(&filename[ft_strlen(filename) - 3], ".rt", 3) != 0)
		call_error("invalid file extension", NULL, NULL);
	data->objects._file_fd = open(filename, O_RDONLY);
	if (data->objects._file_fd == -1)
	{
		perror(filename);
		exit(1);
	}
	init_light_struct(data);
	parse_line_loop(data);
	data->objects._tokens = NULL;
	if (data->cam.aspect_ratio == 0)
		call_error("There has to be a camera object!!!", "parse_input",
			&data->objects);
	data->world = hittablelist(data->objects.hit_list, data->objects.hit_idx);
	data->lights = hittablelist(data->objects.light_hit,
			data->objects.light_idx);
	if (close(data->objects._file_fd) == -1)
		perror("MiniRT: close()");
}
