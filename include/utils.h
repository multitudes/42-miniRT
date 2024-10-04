/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 18:49:26 by lbrusa            #+#    #+#             */
/*   Updated: 2024/09/30 10:16:46 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

#include <math.h>
# ifndef PI
#  define PI 3.1415926535897932385
# endif

# include "vec3.h"
# include "color.h"
# include <stdlib.h>

// Epsilon value for floating-point comparison
# define EPSILON 1e-6
// #define CORES 16
# define RANDOM_SYSTEM 0

// Define constants for the LCG
// #define A 1664525
// #define C 1013904223
// #define M 4294967296 // 2^32

# define A 1103515245
# define C 1013904223
# define M 4294967296 // 2^32

/*	put this in utils, because it is an underused header -
	will not cause circular depencancies */
typedef struct s_init_params
{
	t_point3	center;
	t_vec3		normal;
	double		diam;
	double		height;
	t_rgb		rgbcolor;
	t_vec3		side1;		// quad
	t_vec3		side2;		// quad
	t_point3	a;			// triangle
	t_point3	b;			// triangle
	t_point3	c;			// triangle
	t_material	*mat;
}				t_init_params;


unsigned int	rand_rt(void);

/* Comverts degrees to radians. */
double			degrees_to_radians(double degrees);

/* Returns a random real in [0,1). */
double			random_d(void);

/* Returns a random real in [min,max). */
double			random_double(double min, double max);
int				random_int(int min, int max);

#endif
