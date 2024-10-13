/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 18:49:26 by lbrusa            #+#    #+#             */
/*   Updated: 2024/09/19 17:01:37 by lbrusa           ###   ########.fr       */
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

/**
 * @brief Our random int generator.
 */
unsigned int rand_rt();

/*
 * Comverts degrees to radians.
 */
double degrees_to_radians(double degrees);

/*
 * Returns a random double in [0,1], 1 excluded.
 */
double random_d();

/*
 * Returns a random double in [min,max]
 */
double random_double(double min, double max);

/**
 * @brief Returns a random int in [min,max] max excluded
 */
int random_int(int min, int max);

#endif