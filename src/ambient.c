/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ambient.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 11:53:45 by lbrusa            #+#    #+#             */
/*   Updated: 2024/09/24 11:39:34 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "ambient.h"
#include "color.h"

void	ambient(t_ambient *result, double ratio, t_rgb rgb)
{
	result->ratio = clamp(interval(0,1), ratio);
	result->rgbcolor = rgb;
	result->color = vec3multscalar(rgb_to_color(rgb), ratio);
	result->print = &print_ambient;
}

void		print_ambient(const void *self)
{
	const t_ambient *ambient = self;
	printf("A\t%.f\t\t%d,%d,%d\n",ambient->ratio,
	ambient->rgbcolor.r, ambient->rgbcolor.g, ambient->rgbcolor.b);
}
