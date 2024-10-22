/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 11:17:39 by lbrusa            #+#    #+#             */
/*   Updated: 2024/10/14 17:12:41 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

/*
 getops_nr returns which op to call from the array of pointer 
 to functions g_ops. ex: I have %5.5s so I look up the 
 position of s in the array of conversion and I have 2. 
 Therefore I call the second function in the array which 
 happens to be pr_s
 As arguments I pass ii which is the pointer to the counter to update it
 and update also the format string which will be
 updated in the function. 
 p is the pointer to the main string (format) starting from the next char
 where I had a '%' char. format_str needs to be freed when finished.  
 */
int	getops_nr(char *s, char **conv, int *ii)
{
	int		i;
	char	*r;

	i = 0;
	r = 0;
	while (s[i])
	{
		r = ft_strchr(CONVS, (char)(s[i]));
		if (r != NULL)
		{
			*conv = ft_substr(s, 0, i);
			*ii += i + 1;
			return ((int)(r - CONVS));
		}
		if (ft_isalpha(s[i]))
			return (CONV_ERROR);
		i++;
	}
	return (CONV_ERROR);
}

/*
I will look for the correct conversion function in the array and 
launch it getting back the number of chars written
ops[CONV_ERROR] just prints the original sonversion string.
ex if I have %4.4r which is not found as a valid conv then I just 
print %4.4r and not an error message(like the orig printf)
 */
ssize_t	get_conv(char *format, int *ii, va_list *ap)
{
	ssize_t	n;
	int		opnr;
	char	*conv;
	t_ffunc	ops[MAX_OPS];

	n = 0;
	++(*ii);
	init_ops(ops);
	conv = NULL;
	opnr = getops_nr(format + (*ii), &conv, ii);
	if (!conv)
		return (ops[CONV_ERROR](ap, conv));
	if (opnr == CONV_ERROR)
		return (ops[CONV_ERROR](ap, conv));
	n = ops[opnr](ap, conv);
	free(conv);
	return (n);
}

int	ft_printf(const char *format, ...)
{
	ssize_t	n;
	int		i;
	va_list	ap;

	i = 0;
	n = 0;
	if (!format)
		return (-1);
	va_start(ap, format);
	while (format[i])
	{
		if (format[i] == '%')
			n += get_conv((char *)format, &i, &ap);
		else
			n += write(1, format + i++, 1);
	}
	va_end(ap);
	return (n);
}
