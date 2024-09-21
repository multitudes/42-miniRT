/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 18:49:10 by lbrusa            #+#    #+#             */
/*   Updated: 2024/09/21 11:09:01 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "camera.h"

#include "utils.h"



/**
 *  @brief Our random int generator.
 *
 * @return unsigned int
 */
unsigned int rand_rt()
{
	static __thread unsigned int seed = 1;
    seed = (A * seed + C) % M;
    return seed;
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
	int	diff;

	diff = max - min;
	if (diff <= 0)
		diff = 1;
    if (CORES > 1)
    {
        unsigned int seed  = rand();
        return min + rand_r(&seed) % diff; // maybe remove and use our random func
    }
    return min + rand_rt() % diff;
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
