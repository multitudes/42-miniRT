/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 18:49:26 by lbrusa            #+#    #+#             */
/*   Updated: 2024/09/14 18:27:36 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

#include <math.h>
#include <stdint.h>
#include <stdlib.h>

#ifndef PI
# define PI 3.1415926535897932385
#endif

// Epsilon value for floating-point comparison
#define EPSILON 1e-6

// Define constants for the LCG
#define A 1664525
#define C 1013904223
#define M 4294967296 // 2^32

unsigned int rand_rt();

/*
 * Comverts degrees to radians.
 */
double degrees_to_radians(double degrees);

/*
 * Returns a random real in [0,1).
 */
double random_d();

/*
 * Returns a random real in [min,max).
 */
double random_double(double min, double max);

int random_int(int min, int max);

#endif