/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 18:49:10 by lbrusa            #+#    #+#             */
/*   Updated: 2024/09/06 10:33:40 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

/**
 *  @brief Our random int generator.
 * 
 * @return unsigned int
 */
unsigned int rand_rt() 
{
	static unsigned int seed = 1;
    seed = (A * seed + C) % M;
    return seed;
}

/*
 * Comverts degrees to radians.
 */
double degrees_to_radians(double degrees) {
    return degrees * PI / 180.0;
}

/*
 * Returns a random real in [0,1).
 */
double random_d() {
    return rand_rt() / (UINT32_MAX + 1.0);
}

/*
 * Returns a random real in [min,max).
 */
double random_double(double min, double max) {
    return min + (max-min)*random_d();
}