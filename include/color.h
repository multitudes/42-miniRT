/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 14:47:37 by lbrusa            #+#    #+#             */
/*   Updated: 2024/09/22 14:30:26 by lbrusa           ###   ########.fr       */
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
 * using uint8_t to save and dont have values of more than 255
 */
typedef struct 	s_rgb 
{
    uint32_t 	r;
    uint32_t 	g;
    uint32_t 	b;
} 				t_rgb;

t_color 	    color(double, double, double);
t_rgb           rgb(uint32_t r, uint32_t g, uint32_t b);
double 		    linear_to_gamma(double linear_component);
t_color		    color_random();
t_color		    color_random_min_max(double min, double max);
t_color		    rgb_to_color(t_rgb rgbcolor);
t_rgb           color_to_rgb(t_color color);
unsigned int    rgb_to_uint(t_rgb rgbcolor);

#endif
