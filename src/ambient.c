/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ambient.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 11:53:45 by lbrusa            #+#    #+#             */
/*   Updated: 2024/09/08 12:08:41 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "ambient.h"
#include "color.h"

t_ambient ambient(double ratio, t_rgb rgb)
{
	t_ambient result;

	result.ratio = clamp(interval(0,1), ratio);
	result.rgbcolor = rgb;
	result.color = vec3multscalar(rgb_to_color(rgb), ratio);
	result.print = &print_ambient;
	return result;
}

void		print_ambient(const void *self)
{
	const t_ambient *ambient = self;
	printf("A\t%.f\t\t%d,%d,%d\n",ambient->ratio, 
	ambient->rgbcolor.r, ambient->rgbcolor.g, ambient->rgbcolor.b);
}
