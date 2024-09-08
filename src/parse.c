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
#include <stdint.h>
#include <stdio.h>  /* perror() */
#include "minirt.h"

/* program will exit if there is an error */
int print_error(char *msg)
{
	write(2, "ERROR: ", 7);
	write(2, msg, ft_strlen(msg));

	// TODO: may need to close open file first
	exit(1);
}

int	count_tokens(char **tokens)
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

t_ambient	get_ambient(char **tokens)
{
	t_ambient	data;
	// int			rbg[3];
	// int			i;

	// first check token len
	if (count_tokens(tokens) != 3)
		print_error("ambient: invalid token amount\n");

	// TODO: need string to double conversion
	data.ratio = ft_atod(tokens[1]);
	printf("data.ratio: %f\n", data.ratio);
	return (data);
}


int	update_struct(t_objects *obj, char **tokens)
{
	if (ft_strncmp("A", tokens[0], 2) == 0)
		obj->ambient = get_ambient(tokens);
	else if (ft_strncmp("C", tokens[0], 2) == 0)
	{

	}
	else if (ft_strncmp("L", tokens[0], 2) == 0)
	{

	}
	else if (ft_strncmp("sp", tokens[0], 3) == 0)
	{

	}
	else if (ft_strncmp("pl", tokens[0], 3) == 0)
	{

	}
	else if (ft_strncmp("cy", tokens[0], 3) == 0)
	{

	}
	else
		return(print_error("invalid objects identifier\n"));
	return (0);
}

// TODO: this will return a struct (we dont have memset)
/* in case or error, the parser calls exit() */
void	parse_input(char *filename, t_objects *obj)
{
    int	    fd;
    char    *line;
    char    **tokens;

    // should check for extension
    char	*nothing = &filename[ft_strlen(filename) - 4];
    (void) nothing;
    if (ft_strncmp(&filename[ft_strlen(filename) - 3], ".rt", 3) != 0)
		print_error("invalid file extension\n");

    fd = open(filename, O_RDONLY);
    if (fd == -1)
    	perror("MiniRT: open()"), exit(1);
    while ((line = get_next_line(fd)) != NULL)
    {
		// so if there is just a newline
		if (ft_strlen(line) == 1)
		    continue ;

		tokens = ft_split(line, ' ');
		if (tokens == NULL)
			print_error("parser ft_split\n");
		update_struct(obj, tokens);
		free(tokens);
    }
    if (close(fd) == -1)
    	perror("MiniRT: close()");
}
