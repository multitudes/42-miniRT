/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ralgaran <ralgaran@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 21:23:46 by ralgaran          #+#    #+#             */
/*   Updated: 2023/12/12 12:22:26 by ralgaran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

// includes
# include <stdlib.h>		// malloc, free, NULL, size_t
# include <unistd.h>		// read

// macros
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 256
# endif

// functions
int		ft_my_strlen(char	*str);
char	*get_next_line(int fd);
char	*make_new_line(char *line, char *buffer);
void	ft_strcpy(char *dest, char *src);
void	ft_strcpy_until_newline(char *line, char *buffer, int *line_valid);
void	make_new_remainder(char *remaider, char *buffer);

#endif
