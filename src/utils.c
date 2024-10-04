/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 18:49:10 by lbrusa            #+#    #+#             */
/*   Updated: 2024/10/04 14:24:11 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mersenne_twister.h"
#include "utils.h"
#include <time.h>

/**
 *  @brief Our random int generators.
 *
 * @return unsigned int
 *
 * We have three different random number generators to test
 * the performance. The first one is a simple linear congruential
 * generator (LCG) that i got from copilot. With the
 * __thread keyword, we make sure that each thread has its own
 * seed. It is very fast I think it has a good randomness.
 * The second one is the rand() function from the C standard library
 * which is so slow when used in a multithreaded environment that
 * it is actually a true bottleneck. It is not thread safe.
 * The last is the Mersenne Twister which is a very good random
 * number generator. It is also thread safe. The problem is that
 * the effects on the minirt images are somewhat unpredictable.
 * the shadows disappear for instance.
 */
#if RANDOM_SYSTEM == 0

unsigned int	rand_rt(void)
{
	static __thread unsigned int	seed = 1;

	seed = (A * seed + C) % M;
	return (seed);
}
#elif RANDOM_SYSTEM == 1

unsigned int	rand_rt(void)
{
	static __thread int	initialized = 0;

	if (!initialized)
	{
		srand(time(NULL) ^ (uintptr_t) & initialized);
		initialized = 1;
	}
	return (rand());
}
#elif RANDOM_SYSTEM == 2

unsigned int	rand_rt(void)
{
	return (mt_genrand_int32());
}
#endif

// int get_num_cores(void) 
// {
//     long	num_cores;

// 	num_cores = sysconf(_SC_NPROCESSORS_ONLN);
//     if (num_cores == -1) {
//         perror("sysconf");
//         exit(EXIT_FAILURE);
//     }
//     return (int)num_cores;
// }