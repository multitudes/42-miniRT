/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ralgaran <ralgaran@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/08 14:41:34 by ralgaran          #+#    #+#             */
/*   Updated: 2023/12/12 13:50:41 by ralgaran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

/**
* =====================================
*	returns the strlen of a string
* =====================================
* inputs -> string
* ------------------------------------
* success out -> (int)len
* ------------------------------------
*/
int	ft_my_strlen(char *str)
{
	int	len;

	len = 0;
	while (*str)
	{
		str++;
		len++;
	}
	return (len);
}

/**
* =====================================
*	copies a string and null terminates result
* =====================================
* inputs -> char *dest, char *src
* ------------------------------------
* success out -> void
* ------------------------------------
*/
void	ft_strcpy(char *dest, char *src)
{
	while (*src)
		*dest++ = *src++;
	*dest = '\0';
}

/**
* =====================================
*	copies a string until a newline, null terminates result
*	(if something added to the line, changes line_valid flag)
* =====================================
* inputs -> dest, src, line_valid
* ------------------------------------
* success out -> void
* ------------------------------------
*/
void	ft_strcpy_until_newline(char *line, char *buffer, int *line_valid)
{
	while (*buffer && *buffer != '\n')
	{
		*line++ = *buffer++;
		*line_valid = 1;
	}
	if (*buffer == '\n')
		*line++ = *buffer;
	*line = '\0';
}

/**
* =====================================
*	combines old line and buffer, to make
*	a new line. frees old line.
* =====================================
* inputs -> old line, buffer
* ------------------------------------
* success out -> new_line (malloced)
* error out -> null(malloc err.)
* ------------------------------------
*/
char	*make_new_line(char *line, char *buffer)
{
	int		i;
	int		old_line_len;
	int		buffer_len;
	char	*new_line;

	old_line_len = ft_my_strlen(line);
	buffer_len = ft_my_strlen(buffer);
	new_line = malloc((old_line_len + buffer_len + 1) * sizeof(char));
	if (!new_line)
		return (NULL);
	i = -1;
	while (line[++i])
		new_line[i] = line[i];
	free(line);
	while (*buffer && *buffer != '\n')
		new_line[i++] = *buffer++;
	if (*buffer == '\n')
		new_line[i++] = *buffer;
	new_line[i] = '\0';
	return (new_line);
}

/**
* =====================================
*	makes new remainder from the last buffer
*	read (the buffer will contain a newline).
*	new remainder = everything after 1st newline.
* =====================================
* inputs -> remainder, buffer
* ------------------------------------
* success out -> void (remainder changed in place)
* ------------------------------------
*/
void	make_new_remainder(char *remainder, char *buffer)
{
	int	newline_found;
	int	added_to_remainder_count;

	added_to_remainder_count = 0;
	newline_found = 0;
	while (*buffer)
	{
		if (newline_found)
		{
			*remainder++ = *buffer;
			added_to_remainder_count++;
		}
		if (!newline_found && *buffer == '\n')
			newline_found = 1;
		buffer++;
	}
	while (added_to_remainder_count++ < BUFFER_SIZE)
		*remainder++ = '\0';
}
