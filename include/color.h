/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 14:47:37 by lbrusa            #+#    #+#             */
/*   Updated: 2024/09/06 20:23:01 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COLOR_H
# define COLOR_H

# include "vec3.h"
# include <stdint.h>
# include <math.h>
#include "ray.h"
#include "hittable_list.h"

/**
 * @brief A color is already an alias for a vec3
 * but I get rgb from the .rt file therefore i create a struct for it
 */
typedef struct 	s_rgb 
{
    uint8_t 	r;
    uint8_t 	g;
    uint8_t 	b;
} 				t_rgb;

t_color 	color(double, double, double);
t_rgb 		rgb(int r, int g, int b);
double 		linear_to_gamma(double linear_component);
t_color		color_random();
t_color		color_random_min_max(double min, double max);
t_color		rgb_to_color(t_rgb rgbcolor);
#endif