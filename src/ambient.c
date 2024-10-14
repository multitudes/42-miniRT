/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ambient.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 11:53:45 by lbrusa            #+#    #+#             */
/*   Updated: 2024/10/14 13:57:26 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ambient.h"
#include "color.h"
#include "debug.h"
#include <stdio.h>

void	ambient(t_ambient *result, double ratio, t_rgb rgb)
{
	result->ratio = ratio;
	result->rgbcolor = rgb;
	result->color = vec3multscalar(rgb_to_color(rgb), ratio);
}
