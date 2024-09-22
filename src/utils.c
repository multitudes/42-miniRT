/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 18:49:10 by lbrusa            #+#    #+#             */
/*   Updated: 2024/09/22 16:43:28 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"
#include "mersenne_twister.h"
#include <time.h>
#define CORES 16
#define RANDOM_SYSTEM 0

/**
 *  @brief Our random int generator.
 * 
 * @return unsigned int
 */
#if RANDOM_SYSTEM == 0
unsigned int rand_rt() 
{
	static __thread unsigned int seed = 1;
    seed = (A * seed + C) % M;
    return (seed);
}
#elif RANDOM_SYSTEM == 1
unsigned int rand_rt() 
{
    static __thread int initialized = 0;
    if (!initialized) {
        srand(time(NULL) ^ (uintptr_t)&initialized); // Seed the random number generator with a unique seed per thread
        initialized = 1;
    }
    return rand();
}
#elif RANDOM_SYSTEM == 2
unsigned int rand_rt()
{
    return mt_genrand_int32();
}
#endif

/*
 * Comverts degrees to radians.
 */
double degrees_to_radians(double degrees) 
{
    return degrees * PI / 180.0;
}

/*
 * @brief Returns a random int in [min,max).
 * 
 * max is excluded.
 */
int random_int(int min, int max) 
{
    return min + rand_rt() % (max - min);
}

/*
 * @brief Returns a random real in [0,1), 1 excluded.
 */
double random_d() 
{
    return rand_rt() / (UINT32_MAX + 1.0);
}

/*
 * @brief Returns a random real in [min,max) 
 * and max excluded.
 */
double random_double(double min, double max) 
{
    return min + (max-min)*random_d();
}