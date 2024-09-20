/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 18:49:10 by lbrusa            #+#    #+#             */
/*   Updated: 2024/09/20 15:02:54 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "camera.h"

#include "utils.h"
#define CORES 16


/**
 *  @brief Our random int generator.
 * 
 * @return unsigned int
 */
unsigned int rand_rt() 
{
	static __thread unsigned int seed = 1;
    seed = (A * seed + C) % M;
    return (seed);
}

/*
 * Comverts degrees to radians.
 */
double degrees_to_radians(double degrees) 
{
    return degrees * PI / 180.0;
}

/*
 * Returns a random int in [min,max).
 * max is excluded.
 */
int random_int(int min, int max) 
{
    if (CORES > 1)
    {
        unsigned int seed  = rand();
        return min + rand_r(&seed) % (max - min);
    }
    return min + rand_rt() % (max - min);
}
/*
 * Returns a random real in [0,1], 1 excluded.
 */
double random_d() 
{
    return rand_rt() / (UINT32_MAX + 1.0);
}

/*
 * Returns a random real in [min,max) with min included 
 * and max excluded.
 */
double random_double(double min, double max) 
{
    return min + (max-min)*random_d();
}