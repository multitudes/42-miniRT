/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ralgaran <ralgaran@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 21:25:23 by ralgaran          #+#    #+#             */
/*   Updated: 2023/12/12 13:46:07 by ralgaran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

/**
* =====================================
*	checks for newline in a string
* =====================================
* inputs -> string
* ------------------------------------
* success out -> 1 if found
* error out -> 0 if not found
* ------------------------------------
*/
int	check_for_newline(char *str)
{
	int	newline_found;

	newline_found = 0;
	while (*str && !newline_found)
	{
		if (*str == '\n')
			newline_found = 1;
		str++;
	}
	return (newline_found);
}

/**
* =====================================
*	takes two buffers max and if they
*	exist (if they are not NULL) and 
*	frees them, and returns NULL.
* =====================================
* inputs -> buffer1, buffer2
* ------------------------------------
* success out -> NULL
* error out -> -
* ------------------------------------
*/
void	*free_leaks(char *buffer, char *line)
{
	if (buffer)
		free(buffer);
	if (line)
		free(line);
	return (NULL);
}

/**
* =====================================
*	executes some final functions of read_text
*	returns 0 if bytes_read < 0 (read failed)
*	or 
*	if bytes_read == 0 && !line_valid (file is empty)
*	else makes remainder, frees buffer, returns 1
* =====================================
* inputs -> bytes_read, line_valid, buffer, remainder
* ------------------------------------
* success out -> 1
* error out -> 0
* ------------------------------------
*/
int	endgame(int bytes_read, int line_valid, char *buffer, char *remainder)
{
	if (bytes_read < 0)
		return (0);
	else if (bytes_read == 0 && !line_valid)
		return (0);
	else
	{
		make_new_remainder(remainder, buffer);
		free(buffer);
		return (1);
	}
}

/**
* ====================================
* 	uses remainder and reads from a file,
*	if needed, to make a line, which ends
*	with a \n (if not last line). makes new remainder.
* ====================================
* inputs -> fd, remainder (static)
* ------------------------------------
* success out -> line (malloced)
* error out -> NULL (EOF/ read() err./ malloc fail)
* ------------------------------------
*/
char	*read_text(int fd, char *line, char *remainder)
{
	int		line_valid;
	int		bytes_read;
	char	*buffer;

	buffer = malloc ((BUFFER_SIZE + 1) * sizeof(char));
	if (!buffer)
		return (NULL);
	ft_strcpy(buffer, remainder);
	line_valid = 0;
	ft_strcpy_until_newline(line, buffer, &line_valid);
	bytes_read = 1;
	while (!(check_for_newline(line)) && bytes_read > 0)
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read <= 0)
			break ;
		buffer[bytes_read] = '\0';
		line = make_new_line(line, buffer);
		line_valid = 1;
	}
	if (endgame(bytes_read, line_valid, buffer, remainder))
		return (line);
	return (free_leaks(buffer, line));
}

/**
* ====================================
* 	returns a single line from a file
*	(null terminated)
* ====================================
* inputs -> fd (file descriptor)
* ------------------------------------
* success out -> line (malloced)
* error out -> NULL (EOF/ read() err./ malloc fail)
* ------------------------------------
*/
char	*get_next_line(int fd)
{
	static char	remainder[BUFFER_SIZE];
	char		*line;

	if (fd < 0 || fd > 1023)
		return (NULL);
	line = malloc(BUFFER_SIZE * sizeof(char));
	if (!line)
		return (NULL);
	line = read_text(fd, line, remainder);
	return (line);
}
