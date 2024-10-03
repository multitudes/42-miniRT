/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ralgaran <ralgaran@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 16:43:06 by ralgaran          #+#    #+#             */
/*   Updated: 2024/10/03 16:43:08 by ralgaran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

void	free_split(char **split)
{
	int	i;

	i = -1;
	while (split[++i])
		free(split[i]);
	free(split);
}

/* program will exit if there is an error */
int	call_error(char *msg, char *prefix, t_objects *obj)
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
}

int	count_tokens(char **tokens)
{
	int	i;

	i = -1;
	while (tokens[++i])
		;
	return (i);
}

static void	ft_atod_loop(char *str, double *result, int *exponent)
{
	int			i;
	long long	num;

	num = 0;
	i = -1;
	while (str[++i])
	{
		if (str[i] == '.')
		{
			if (*exponent != -1 || ft_isdigit(str[i + 1]) == 0)
				break ;
			*exponent = i;
			str++;
		}
		else if (ft_isdigit(str[i]) == 0)
			break ;
		num = (num * 10) + (int)(str[i] - '0');
	}
	*result = (double)num;
	if (*exponent != -1)
		*exponent = i - *exponent;
}

/* converts a string to a double */
double	ft_atod(char *str)
{
	int			is_negative;
	double		result;
	int			exponent;

	if (!str)
		return (.0);
	is_negative = 0;
	if (*str == '-')
	{
		is_negative = 1;
		str++;
	}
	exponent = -1;
	result = 0;
	ft_atod_loop(str, &result, &exponent);
	if (exponent != -1 && result != 0)
	{
		while (exponent--)
			result /= 10.;
	}
	if (is_negative)
		return (-result);
	return (result);
}
