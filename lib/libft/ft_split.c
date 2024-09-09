/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ralgaran <ralgaran@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 13:21:23 by ralgaran          #+#    #+#             */
/*   Updated: 2024/02/10 13:35:23 by ralgaran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
	char	**ft_split(char const *s, char c)
	-> splits a string into words on the char c
*/

#include "libft.h"		/* malloc(), free(), NULL */

// works
static int	word_count(char const *str, char c)
{
	int	count;
	int	is_split;

	count = 0;
	is_split = 1;
	while (*str)
	{
		if (is_split && *str != c)
		{
			count++;
			is_split = 0;
		}
		else if (!is_split && *str == c)
			is_split = 1;
		str++;
	}
	return (count);
}

// works
static int	word_len(char const *str, char c)
{
	int	len;

	len = 0;
	while (*str)
	{
		if (*str == c)
			break ;
		len++;
		str++;
	}
	return (len);
}

// works
static char	*make_a_string(char const **s, char c)
{
	int		len;
	int		added;
	char	*string;
	char	*string_start;

	len = word_len(*s, c);
	string = malloc ((len + 1) * sizeof(char));
	if (!string)
		return (NULL);
	string_start = string;
	added = 0;
	while (added < len && **s != c)
	{
		*string++ = *(*s)++;
		added++;
	}
	*string = '\0';
	return (string_start);
}

void	*free_array(char **array, int strings)
{
	int	i;

	if (!array)
		return (NULL);
	i = 0;
	while (i < strings)
	{
		if (array[i])
			free(array[i]);
		i++;
	}
	free(array);
	return (NULL);
}

char	**ft_split(char const *s, char c)
{
	int		string_count;
	int		array_index;
	char	*string;
	char	**array;

	string_count = word_count(s, c);
	array = malloc ((string_count + 1) * sizeof(char *));
	if (!array)
		return (NULL);
	array_index = 0;
	while (*s)
	{
		if (*s != c)
		{
			string = make_a_string(&s, c);
			if (!string)
				return (free_array(array, array_index));
			array[array_index++] = string;
		}
		else
			s++;
	}
	array[array_index] = NULL;
	return (array);
}
