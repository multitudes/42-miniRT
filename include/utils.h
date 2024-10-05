/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 18:49:26 by lbrusa            #+#    #+#             */
/*   Updated: 2024/10/05 09:56:49 by lbrusa           ###   ########.fr       */
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
# define CORES 20
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
	t_point3	a;			// triangle		// box
	t_point3	b;			// triangle		// box
	t_point3	c;			// triangle
	t_material	*mat;
}				t_init_params;


uint32_t		rand_rt(void);
double			degrees_to_radians(double degrees);
double			random_d(void);
double			random_double(double min, double max);
int				random_int(int min, int max);
// int				get_num_cores(void); 

#endif
